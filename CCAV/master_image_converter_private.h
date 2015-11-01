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


#ifndef CCAV_IMAGECONVERTER_P_H
#define CCAV_IMAGECONVERTER_P_H

#include <CCAV/master_global.h>
#include "master_array.h"
#include <CCAV/compat_common.h>
#define PIX_FMT PIX_FMT_RGB32 //PIX_FMT_YUV420P

namespace CCAV {

class CCImageConverter;
class Q_EXPORT CCImageConverterPrivate : public DPtrPrivate<CCImageConverter>
{
public:
	CCImageConverterPrivate() :interlaced(false), data_outs(10, 'c'), w_in(0), h_in(0), w_out(0), h_out(0)
		, fmt_in(PIX_FMT_YUV420P), fmt_out(PIX_FMT_RGB32)
	{}
    bool interlaced;
    int w_in, h_in, w_out, h_out;
    int fmt_in, fmt_out;
	ByteArray data_outs;
};

} //namespace CCAV
#endif // CCAV_IMAGECONVERTER_P_H
