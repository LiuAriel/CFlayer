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


#ifndef CCAV_IMAGECONVERTER_H
#define CCAV_IMAGECONVERTER_H

#include <CCAV/master_global.h>
#include "master_array.h"
namespace CCAV {

class CCImageConverterPrivate;
class CCImageConverter
{
    DPTR_DECLARE_PRIVATE(CCImageConverter)
public:
    CCImageConverter();

	ByteArray out_data() const;
    void set_in_size(int width, int height);
    void set_out_size(int width, int height);
    void set_in_format(int format);
    void set_out_format(int format);
    void set_interlaced(bool interlaced);
    bool is_interlaced() const;
    virtual bool convert(const quint8 *const srcSlice[], const int srcStride[]) = 0;
protected:
    CCImageConverter(CCImageConverterPrivate& d);
	virtual bool prepare_data();
    DPTR_DECLARE(CCImageConverter)
};

} //namespace CCAV
#endif // CCAV_IMAGECONVERTER_H
