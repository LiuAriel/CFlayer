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

#include <CCAV/decode_video.h>
#include <private/master_decode_private.h>
#include <CCAV/master_image_converter_ffmpeg.h>
#include <CCAV/master_packet.h>
#include <CCAV/compat_common.h>
#include <QtCore/QSize>

#if CONFIG_EZX
#define PIX_FMT PIX_FMT_BGR565
#else
#define PIX_FMT PIX_FMT_RGB32 //PIX_FMT_YUV420P
#endif //CONFIG_EZX

namespace CCAV {

class CCDecodeVideoPrivate : public CCDevoderPrivate
{
public:
    CCDecodeVideoPrivate():width(0),height(0)
    {
        conv = new CCImageConverterFF(); 
        conv->set_out_format(PIX_FMT_RGB32);
    }
    ~CCDecodeVideoPrivate() {
        if (conv) {
            delete conv;
            conv = 0;
        }
    }

    int width, height;
    CCImageConverter* conv;
};

CCDecodeVideo::CCDecodeVideo()
    :CCDevoder(*new CCDecodeVideoPrivate())
{
}

bool CCDecodeVideo::decode(const ByteArray& encoded)
{
	if (!is_available())
		return false;
	DPTR_D(CCDecodeVideo);
	AVPacket packet;
	av_new_packet(&packet, encoded.size());
	memcpy(packet.data, encoded.data(), encoded.size());
	
	int ret = avcodec_decode_video2(d.codec_ctx, d.frame, &d.got_frame_ptr, &packet);
	
	av_free_packet(&packet);
	if (ret < 0) {
		return false;
	}
	if (!d.got_frame_ptr) {
		return false;
	}

	if (d.width <= 0 || d.height <= 0) {
		qDebug("decoded video size not seted. use original size [%d x %d]"
			, d.codec_ctx->width, d.codec_ctx->height);
		if (!d.codec_ctx->width || !d.codec_ctx->height)
			return false;
		resize_video(d.codec_ctx->width, d.codec_ctx->height);
	}
	
	d.conv->set_in_format(d.codec_ctx->pix_fmt);
	d.conv->set_in_size(d.codec_ctx->width, d.codec_ctx->height);
	d.conv->convert(d.frame->data, d.frame->linesize);
	d.decoded_video = d.conv->out_data();
	return true;
}

void CCDecodeVideo::resize_video(const QSize &size)
{
    resize_video(size.width(), size.height());
}

void CCDecodeVideo::resize_video(int width, int height)
{
    if (width == 0 || height == 0)
        return;
    DPTR_D(CCDecodeVideo);
    d.conv->set_out_size(width, height);
    d.width = width;
    d.height = height;
}

int CCDecodeVideo::width() const
{
    return d_func().width;
}

int CCDecodeVideo::height() const
{
    return d_func().height;
}
} //namespace CCAV
