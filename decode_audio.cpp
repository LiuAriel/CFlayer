/******************************************************************************
	CCAV:  Media play library based on Qt(UI),c/c++11 and FFmpeg
	Copyright (C) 2015-2016 Juno <junowendy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <CCAV/decode_audio.h>
#include <private/master_decode_private.h>
#include <CCAV/compat_common.h>
#include "util.h"

namespace CCAV {

class CCDecodeAudioPrivate : public CCDevoderPrivate
{
public:
   
};

CCDecodeAudio::CCDecodeAudio()
    :CCDevoder(*new CCDecodeAudioPrivate)
{

}

bool CCDecodeAudio::decode(const ByteArray& encoded)
{
	if (!is_available())
		return false;
	DPTR_D(CCDecodeAudio);
	
	std::unique_ptr<AVPacket, std::function<void(AVPacket*)>> packet(
		new AVPacket, [](AVPacket* p){ av_free_packet(p); delete p; });
	av_new_packet(packet.get(), encoded.size());
	memcpy(packet->data, encoded.data(), encoded.size());

	int ret = avcodec_decode_audio4(d.codec_ctx, d.frame, &d.got_frame_ptr, packet.get());

	if (ret < 0) {
		
		return false;
	}
	if (!d.got_frame_ptr) {
		qWarning("[CCDecodeAudio] got_frame_ptr=false");
		return false;
	}
	int samples_with_channels = d.frame->nb_samples * d.codec_ctx->channels;
	int samples_with_channels_half = samples_with_channels / 2;
	d.decoded_audio.resize(samples_with_channels * sizeof(float));
	float *decoded_datas = (float*)d.decoded_audio.constData();
	static const float kInt8_inv = 1.0f / 128.0f;
	static const float kInt16_inv = 1.0f / 32768.0f;
	static const float kInt32_inv = 1.0f / 2147483648.0f;
	
	switch (d.codec_ctx->sample_fmt) {
	case AV_SAMPLE_FMT_U8:
	{
		uint8_t *data = (uint8_t*)*d.frame->data;
		for (int i = 0; i < samples_with_channels_half; i++) {
			decoded_datas[i] = (data[i] - 0x7F) * kInt8_inv;
			decoded_datas[samples_with_channels - i] = (data[samples_with_channels - i] - 0x7F) * kInt8_inv;
		}
		break;
	}
	case AV_SAMPLE_FMT_S16:
	{
		int16_t *data = (int16_t*)*d.frame->data;
		for (int i = 0; i < samples_with_channels; ++i)
		{
			decoded_datas[i] = data[i] * kInt16_inv;
		}
		break;
	}
	case AV_SAMPLE_FMT_S32:
	{
		int32_t *data = (int32_t*)*d.frame->data;
		for (int i = 0; i < samples_with_channels_half; i++) {
			decoded_datas[i] = data[i] * kInt32_inv;
			decoded_datas[samples_with_channels - i] = data[samples_with_channels - i] * kInt32_inv;
		}
		break;
	}
	case AV_SAMPLE_FMT_FLT:
	{
		memcpy(decoded_datas, *d.frame->data, d.decoded_audio.size());
		break;
	}
	case AV_SAMPLE_FMT_DBL:
	{
		double *data = (double*)*d.frame->data;
		for (int i = 0; i < samples_with_channels_half; i++) {
			decoded_datas[i] = data[i];
			decoded_datas[samples_with_channels - i] = data[samples_with_channels - i];
		}
		break;
	}
	case AV_SAMPLE_FMT_U8P:
	{
		uint8_t **data = (uint8_t**)d.frame->extended_data;
		for (int i = 0; i < d.frame->nb_samples; ++i) {
			for (int ch = 0; ch < d.codec_ctx->channels; ++ch) {
				*decoded_datas++ = (data[ch][i] - 0x7F) * kInt8_inv;
			}
		}
		break;
	}
	case AV_SAMPLE_FMT_S16P:
	{
		uint16_t **data = (uint16_t**)d.frame->extended_data;
		for (int i = 0; i < d.frame->nb_samples; ++i) {
			for (int ch = 0; ch < d.codec_ctx->channels; ++ch) {
				*decoded_datas++ = data[ch][i] * kInt16_inv;
			}
		}
		break;
	}
	case AV_SAMPLE_FMT_S32P:
	{
		uint32_t **data = (uint32_t**)d.frame->extended_data;
		for (int i = 0; i < d.frame->nb_samples; ++i) {
			for (int ch = 0; ch < d.codec_ctx->channels; ++ch) {
				*decoded_datas++ = data[ch][i] * kInt32_inv;
			}
		}
		break;
	}
	case AV_SAMPLE_FMT_FLTP:
	{
		float **data = (float**)d.frame->extended_data;
		for (int i = 0; i < d.frame->nb_samples; ++i) {
			for (int ch = 0; ch < d.codec_ctx->channels; ++ch) {
				*decoded_datas++ = data[ch][i];
			}
		}
		break;
	}
	case AV_SAMPLE_FMT_DBLP:
	{
		double **data = (double**)d.frame->extended_data;
		for (int i = 0; i < d.frame->nb_samples; ++i) {
			for (int ch = 0; ch < d.codec_ctx->channels; ++ch) {
				*decoded_datas++ = data[ch][i];
			}
		}
		break;
	}
	default:
		static bool sWarn_a_fmt = true;
		if (sWarn_a_fmt) {
			qWarning("Unsupported audio format: %d", d.codec_ctx->sample_fmt);
			sWarn_a_fmt = false;
		}
		d.decoded_audio.clear();
		break;
	}
	return !d.decoded_audio.isEmpty();
}

} //namespace CCAV
