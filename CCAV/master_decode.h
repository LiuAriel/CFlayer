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

#ifndef CCAV_DECODER_H
#define CCAV_DECODER_H

#include <CCAV/master_global.h>
#include <stdint.h>
#include "CCAV/compat_common.h"
#include "master_array.h"
class QByteArray;
struct AVCodecContext;
struct AVFrame;

namespace CCAV {

class CCDevoderPrivate;
class Q_EXPORT CCDevoder
{
    DPTR_DECLARE_PRIVATE(CCDevoder)
public:
    CCDevoder();
    virtual ~CCDevoder();
    void set_codec_context(AVCodecContext* codecCtx); 
    AVCodecContext* codec_context() const;
    
    bool is_available() const;
	virtual bool decode(const ByteArray& encoded) = 0;
	ByteArray data_video() const; //decoded data
	ByteArray data_audio() const; 
protected:
    CCDevoder(CCDevoderPrivate& d);

    DPTR_DECLARE(CCDevoder)
};

} //namespace CCAV
#endif // CCAV_DECODER_H
