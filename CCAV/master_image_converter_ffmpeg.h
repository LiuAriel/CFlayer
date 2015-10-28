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

#ifndef CCAV_IMAGECONVERTERFF_H
#define CCAV_IMAGECONVERTERFF_H

#include <CCAV/master_image_converter.h>

namespace CCAV {

class CCImageConverterFFPrivate;
class CCImageConverterFF : public CCImageConverter
{
    DPTR_DECLARE_PRIVATE(CCImageConverterFF)
public:
    CCImageConverterFF();
	virtual bool convert(const quint8 *const srcSlice[], const int srcStride[]) override;
protected:
	virtual bool prepare_data();
};
} //namespace CCAV
#endif // CCAV_IMAGECONVERTERFF_H
