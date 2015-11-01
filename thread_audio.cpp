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

#include <CCAV/thread_audio.h>
#include <private/master_thread_private.h>
#include <CCAV/decode_audio.h>
#include <CCAV/master_packet.h>
#include <CCAV/output_audio.h>
#include <CCAV/master_clock.h>
//#include <QtCore/QCoreApplication>
#include <assert.h>
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
#include <libavcodec/avcodec.h>
#include <libavutil/time.h>
#ifdef __cplusplus
}
#endif //__cplusplus

namespace CCAV {

class CCAudioThreadPrivate : public CCThreadPrivate
{
public:
    double last_pts; 
};

CCAudioThread::CCAudioThread()
    :CCThread(*new CCAudioThreadPrivate())
{
}

void CCAudioThread::run_audio()
{
	DPTR_D(CCAudioThread);
	if (!d.dec || !d.dec->is_available())
		return;
	reset_state();
	assert(d.clock != 0);
	CCDecodeAudio *dec = static_cast<CCDecodeAudio*>(d.dec);
	CCOutputAudio *ao = static_cast<CCOutputAudio*>(d.writer);
	int sample_rate = dec->codec_context()->sample_rate;
	int channels = dec->codec_context()->channels;
	int csf = channels * sample_rate * sizeof(float);
	static const double max_len = 0.02;
	d.last_pts = 0;
	while (!d.stop) {
		
		if (try_pause()) {
			if (d.stop)
				break; 
		}
		std::unique_lock<std::mutex> lock(d.mutex_);
		if (d.packets.is_empty() && !d.stop) {
			d.stop = d.demux_end;
			if (d.stop) {
				break;
			}
		}

		PacketData pkt;
		d.packets.pop(pkt); 
		if (pkt.pts <= 0 || pkt.date.isEmpty()) {
			qDebug("Empty packet!");
			dec->flush_codec();
			continue;
		}
		d.clock->update_value(pkt.pts);
		
		ByteArray da(pkt.date);
		if (d.dec->decode(da)) {
			ByteArray decoded_audio = d.dec->data_audio();
			int decodedSizes = decoded_audio.size();
			int decodedPos = 0;
			double delay = 0;
			while (decodedSizes > 0) {
				int chunk = qMin(decodedSizes, int(max_len*csf));
				d.clock->update_delay(delay += (double)chunk / (double)csf);
				ByteArray decodedChunks(chunk, 0); //volume == 0 || mute
				if (ao && ao->is_available()) {
					if (!ao->is_mute()) {
						ByteArray temp(decoded_audio.constData() + decodedPos, chunk);
						decodedChunks = temp;
						double vol = ao->volume();
						if (vol != 1.0) {
						    int len = decodedChunks.size()/sizeof(float); 
						    float *data = (float*)decodedChunks.data();
						    for (int i = 0; i < len; ++i)
						        data[i] *= vol;
						}
					}
					ao->write_data(decodedChunks);
				}
				else {
					static bool sWarn_no_ao = true; 
					if (sWarn_no_ao) {
						qDebug("Audio output not available! msleep(%lu)", (unsigned long)((double)chunk / (double)csf * 1000));
						sWarn_no_ao = false;
					}
					av_usleep((unsigned long)((double)chunk / (double)csf * 1000000.0));
				}
				decodedPos += chunk;
				decodedSizes -= chunk;
			}
		}
		else {
			double dt = pkt.pts - d.last_pts;
			if (abs(dt) > 0.618 || dt < 0) {
				dt = 0;
			}
			av_usleep((unsigned long)(dt*1000000.0));
		}
		d.last_pts = d.clock->value(); 
	}
	qDebug("Audio thread stops running...");
}


} //namespace CCAV
