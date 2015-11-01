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

#include <CCAV/thread_video.h>
#include <private/master_thread_private.h>
#include <CCAV/master_packet.h>
#include <CCAV/master_clock.h>
#include <CCAV/decode_video.h>
#include <CCAV/video_renderer.h>
#include <CCAV/master_image_converter.h>
#include <QtGui/QImage>
#include "master_array.h"

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

const double kSyncThreshold = 0.005; 

class CCVideoThreadPrivate : public CCThreadPrivate
{
public:
    CCVideoThreadPrivate():conv(0),delay(0){}
    CCImageConverter *conv;
    double delay;
    double pts; 
	ByteArray decoded_datas; 
    int width, height;
    QImage image; 
};

CCVideoThread::CCVideoThread() :
    CCThread(*new CCVideoThreadPrivate())
{
}

CCImageConverter* CCVideoThread::set_image_converter(CCImageConverter *converter)
{
    DPTR_D(CCVideoThread);
    CCAV::CCImageConverter* old = d.conv;
    d.conv = converter;
    return old;
}

CCImageConverter* CCVideoThread::image_converter() const
{
    return d_func().conv;
}

QSize CCVideoThread::current_rawimage_size() const
{
    DPTR_D(const CCVideoThread);
    return QSize(d.width, d.height);
}

double CCVideoThread::current_pts() const
{
    return d_func().pts;
}

void CCVideoThread::run_video()
{
	DPTR_D(CCVideoThread);
	
	if (!d.dec || !d.dec->is_available() || !d.writer)
		return;
	reset_state();
	Q_ASSERT(d.clock != 0);
	CCDecodeVideo *dec = static_cast<CCDecodeVideo*>(d.dec);
	CCVideoRenderer* vo = static_cast<CCVideoRenderer*>(d.writer);
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
		
		if (!pkt.is_valid()) {
			qDebug("Invalid pts or empty packet!");
			dec->flush_codec();
			continue;
		}
		d.delay = pkt.pts - d.clock->value();
		
		if (qAbs(d.delay) < 2.718) {
			if (d.delay > kSyncThreshold) { 
				av_usleep(d.delay * 1000000);
			}
			else if (d.delay < -kSyncThreshold) { 
				//continue;
			}
		}
		else { 
			qDebug("delay %f/%f", d.delay, d.clock->value());
			if (d.delay > 0) {
				av_usleep(0.064 * 1000000);
			}
			else {
				continue;
			}
		}

		d.clock->update_video_pts(pkt.pts); 

		bool vo_ok = vo && vo->is_available();
		if (vo_ok) {
			
			if (vo->last_width() > 0 && vo->last_height() > 0 && !vo->scale_in())
				dec->resize_video(vo->last_size());
			else
				vo->set_source_size(dec->width(), dec->height()); //setLastSize()
		}

		ByteArray da(pkt.date);
		if (d.dec->decode(da)) {
			d.pts = pkt.pts;
			d.decoded_datas = d.dec->data_video();
			if (vo_ok) {
				vo->write_data(d.decoded_datas);
			}
			/*d.width = dec->width();
			d.height = dec->height();
			d.decoded_datas = d.dec->data_video();

			if (vo && vo->is_available()) {
				vo->set_source_size(dec->width(), dec->height());
				vo->write_data(d.decoded_datas);
			}*/
		}

		if (vo_ok && !vo->scale_in())
			vo->set_source_size(vo->video_sizes());
	}
	qDebug("Video thread stops running...");
}

} //namespace CCAV
