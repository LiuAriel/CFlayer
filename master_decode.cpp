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

#include <CCAV/master_decode.h>
#include <private/master_decode_private.h>

namespace CCAV {
CCDevoder::CCDevoder()
{
}

CCDevoder::CCDevoder(CCDevoderPrivate &d)
    :DPTR_INIT(&d)
{

}

CCDevoder::~CCDevoder()
{
}

void CCDevoder::flush_codec()
{
	if (is_available())
		avcodec_flush_buffers(d_func().codec_ctx);
}


void CCDevoder::set_codec_context(AVCodecContext *codecCtx)
{
    DPTR_D(CCDevoder);
    d.codec_ctx = codecCtx;
}

AVCodecContext* CCDevoder::codec_context() const
{
    return d_func().codec_ctx;
}

bool CCDevoder::is_available() const
{
    return d_func().codec_ctx != 0;
}

ByteArray CCDevoder::data_video() const
{
	return d_func().decoded_video;
}

ByteArray CCDevoder::data_audio() const
{
	return d_func().decoded_audio;
}

} //namespace CCAV
