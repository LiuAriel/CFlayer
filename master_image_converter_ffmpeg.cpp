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

#include <CCAV/master_image_converter_ffmpeg.h>
#include <private/master_image_converter_private.h>
#include <CCAV/compat_common.h>

namespace CCAV {

class CCImageConverterFFPrivate : public CCImageConverterPrivate
{
public:
    CCImageConverterFFPrivate():sws_ctx(0){}
    ~CCImageConverterFFPrivate() {
        if (sws_ctx) {
            sws_freeContext(sws_ctx);
            sws_ctx = 0;
        }
    }

    SwsContext *sws_ctx;
    AVPicture picture;
};

CCImageConverterFF::CCImageConverterFF()
    :CCImageConverter(*new CCImageConverterFFPrivate())
{
}

bool CCImageConverterFF::convert(const quint8 *const srcSlice[], const int srcStride[])
{
    DPTR_D(CCImageConverterFF);
    if (d.w_out ==0 || d.h_out == 0) {
        if (d.w_in ==0 || d.h_in == 0)
            return false;
        set_out_size(d.w_in, d.h_in);
    }

    d.sws_ctx = sws_getCachedContext(d.sws_ctx
            , d.w_in, d.h_in, (PixelFormat)d.fmt_in
            , d.w_out, d.h_out, (PixelFormat)d.fmt_out
            , (d.w_in == d.w_out && d.h_in == d.h_out) ? SWS_POINT : SWS_FAST_BILINEAR //SWS_BICUBIC
            , NULL, NULL, NULL
            );

    if (!d.sws_ctx)
        return false;

    int result_h = sws_scale(d.sws_ctx, srcSlice, srcStride, 0, d.h_in, d.picture.data, d.picture.linesize);
    if (is_interlaced()) {
        avpicture_deinterlace(&d.picture, &d.picture, (PixelFormat)d.fmt_out, d.w_out, d.h_out);
    }
    Q_UNUSED(result_h);
    return true;
}

bool CCImageConverterFF::prepare_data()
{
	DPTR_D(CCImageConverterFF);
	int bytes = avpicture_get_size((PixelFormat)d.fmt_out, d.w_out, d.h_out);
	if (d.data_outs.size() < bytes) {
		d.data_outs.resize(bytes);
	}
	
	avpicture_fill(
		&d.picture,
		reinterpret_cast<uint8_t*>(d.data_outs.data()),
		(PixelFormat)d.fmt_out,
		d.w_out,
		d.h_out
		);
	return true;
}

} //namespace CCAV
