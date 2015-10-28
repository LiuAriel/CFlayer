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

#ifndef CCAV_AVDECODER_P_H
#define CCAV_AVDECODER_P_H

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#ifdef __cplusplus
}
#endif //__cplusplus

#include <CCAV/master_global.h>
#include "master_array.h"
namespace CCAV {

class Q_EXPORT CCDevoderPrivate : public DPtrPrivate<CCDevoder>
{
public:
	CCDevoderPrivate() :codec_ctx(0), frame(0), decoded_audio(10, 'c') {
        frame = avcodec_alloc_frame();
    }
    virtual ~CCDevoderPrivate() {
        if (frame) {
            av_free(frame);
            frame = 0;
        }
    }

    AVCodecContext *codec_ctx; 
    AVFrame *frame; 
	ByteArray decoded_video;
	ByteArray decoded_audio;
    int got_frame_ptr;
};

} //namespace CCAV
#endif // CCAV_AVDECODER_P_H
