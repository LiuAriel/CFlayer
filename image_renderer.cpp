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

#include <CCAV/image_renderer.h>
#include <private/image_renderer_private.h>

namespace CCAV {

CCImageRenderer::CCImageRenderer()
    :CCVideoRenderer(*new CCImageRendererPrivate())
{
}

CCImageRenderer::CCImageRenderer(CCImageRendererPrivate &d)
    :CCVideoRenderer(d)
{
}

CCImageRenderer::~CCImageRenderer()
{
}

QImage CCImageRenderer::current_frame_image() const
{
    return d_func().image;
}

void CCImageRenderer::convert_data(const ByteArray &data)
{
	DPTR_D(CCImageRenderer);
#if QT_VERSION >= QT_VERSION_CHECK(4, 0, 0)
	d.image = *&QImage((uchar*)data.data(), d.src_width, d.src_height, QImage::Format_RGB32);
#else
	d.image = QImage((uchar*)data.data(), d.src_width, d.src_height, 16, NULL, 0, QImage::IgnoreEndian);
#endif
}


void CCImageRenderer::set_preview(const QImage &preivew)
{
    DPTR_D(CCImageRenderer);
    d.preview = preivew;
    d.image = preivew;
}

QImage CCImageRenderer::preview_image() const
{
    return d_func().preview;
}

QImage CCImageRenderer::current_image() const
{
    return d_func().image;
}

} //namespace CCAV
