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

#include <CCAV/video_renderer.h>
#include <CCAV/decode_video.h>
#include <private/video_renderer_private.h>
#include <QtCore/QCoreApplication>
#include <CCAV/master_event_filter.h>

namespace CCAV {

CCVideoRenderer::CCVideoRenderer()
    :CCOutput(*new CCVideoRendererPrivate)
{
}

CCVideoRenderer::CCVideoRenderer(CCVideoRendererPrivate &d)
    :CCOutput(d)
{
}

CCVideoRenderer::~CCVideoRenderer()
{
}

void CCVideoRenderer::scale_in(bool q)
{
	d_func().scale_in = q;
}

bool CCVideoRenderer::scale_in() const
{
	return d_func().scale_in;
}

void CCVideoRenderer::set_source_size(const QSize& s)
{
	set_source_size(s.width(), s.height());
}

QSize CCVideoRenderer::last_size() const
{
	DPTR_D(const CCVideoRenderer);
	return QSize(d.src_width, d.src_height);
}

int CCVideoRenderer::last_width() const
{
	DPTR_D(const CCVideoRenderer);
	return d.src_width;
}
int CCVideoRenderer::last_height() const
{
	DPTR_D(const CCVideoRenderer);
	return  d.src_height;
}

void CCVideoRenderer::set_source_size(int width, int height)
{
    DPTR_D(CCVideoRenderer);
    d.src_width = width;
    d.src_height = height;
}

void CCVideoRenderer::registerEventFilter(EventFilter *filter)
{
	DPTR_D(CCVideoRenderer);
	qApp->removeEventFilter(d.event_filter);
	d.event_filter = filter;
	qApp->installEventFilter(filter);
}

bool CCVideoRenderer::open()
{
    return true;
}

bool CCVideoRenderer::close()
{
    return true;
}

void CCVideoRenderer::resize_video(const QSize &size)
{
    resize_video(size.width(), size.height());
}

void CCVideoRenderer::resize_video(int width, int height)
{
    DPTR_D(CCVideoRenderer);
    if (width == 0 || height == 0)
        return;

    d.width = width;
    d.height = height;
}

QSize CCVideoRenderer::video_sizes() const
{
    return QSize(d_func().width, d_func().height);
}

int CCVideoRenderer::video_widths() const
{
    return d_func().width;
}

int CCVideoRenderer::video_heights() const
{
    return d_func().height;
}

} //namespace CCAV
