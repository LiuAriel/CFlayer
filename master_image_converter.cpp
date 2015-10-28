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


#include "master_image_converter.h"
#include <private/master_image_converter_private.h>
#include <CCAV/compat_common.h>

namespace CCAV {

CCImageConverter::CCImageConverter()
{
}

CCImageConverter::CCImageConverter(CCImageConverterPrivate& d):DPTR_INIT(&d)
{
}

ByteArray CCImageConverter::out_data() const
{
	return d_func().data_outs;
}

void CCImageConverter::set_in_size(int width, int height)
{
    d_func().w_in = width;
    d_func().h_in = height;
}

void CCImageConverter::set_out_size(int width, int height)
{
    d_func().w_out = width;
    d_func().h_out = height;
	prepare_data();
}

void CCImageConverter::set_in_format(int format)
{
    d_func().fmt_in = format;
}

void CCImageConverter::set_out_format(int format)
{
    d_func().fmt_out = format;
}

void CCImageConverter::set_interlaced(bool interlaced)
{
    d_func().interlaced = interlaced;
}

bool CCImageConverter::is_interlaced() const
{
    return d_func().interlaced;
}

bool CCImageConverter::prepare_data()
{
    return false;
}

} //namespace CCAV
