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

#ifndef CCAV_VIDEODECODER_H
#define CCAV_VIDEODECODER_H

#include <CCAV/master_decode.h>
#include <stdint.h>
#include "CCAV/compat_common.h"
#include "master_array.h"
class QSize;
struct SwsContext;
namespace CCAV {
class CCDecodeVideoPrivate;
class Q_EXPORT CCDecodeVideo : public CCDevoder
{
    DPTR_DECLARE_PRIVATE(CCDecodeVideo)
public:
    CCDecodeVideo();
	virtual bool decode(const ByteArray& encoded) override;
    void resize_video(const QSize& size);
    void resize_video(int width, int height);

    int width() const;
    int height() const;
};

} //namespace CCAV
#endif // CCAV_VIDEODECODER_H
