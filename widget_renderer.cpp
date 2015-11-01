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

#include <CCAV/widget_renderer.h>
#include <private/widget_renderer_private.h>
#include <qfont.h>
#include <qevent.h>
#include <qpainter.h>
#include <CCAV/master_event_filter.h>

namespace CCAV {
CCWidgetRenderer::CCWidgetRenderer(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),CCImageRenderer(*new CCWidgetRendererPrivate())
{
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(false);
}

CCWidgetRenderer::CCWidgetRenderer(CCWidgetRendererPrivate &d, QWidget *parent, Qt::WindowFlags f)
    :QWidget(parent, f),CCImageRenderer(d)
{
}

CCWidgetRenderer::~CCWidgetRenderer()
{
}

// void CCWidgetRenderer::registerEventFilter(EventFilter *filter)
// {
//     d_func().event_filter = filter;
//     installEventFilter(filter);
// }

bool CCWidgetRenderer::write()
{
	update();
	return true;
}

void CCWidgetRenderer::resizeEvent(QResizeEvent *e)
{
    resize_video(e->size());
    update();
}

void CCWidgetRenderer::mousePressEvent(QMouseEvent *e)
{
    DPTR_D(CCWidgetRenderer);
    d.gMousePos = e->globalPos();
    d.iMousePos = e->pos();
}

void CCWidgetRenderer::mouseMoveEvent(QMouseEvent *e)
{
    if (parentWidget())
        return;
    DPTR_D(CCWidgetRenderer);
    int x = pos().x();
    int y = pos().y();
    int dx = e->globalPos().x() - d.gMousePos.x();
    int dy = e->globalPos().y() - d.gMousePos.y();
    d.gMousePos = e->globalPos();
    int w = width();
    int h = height();
    switch (d.action) {
    case GestureMove:
        x += dx;
        y += dy;
        move(x, y);
        break;
    case GestureResize:
        if(d.iMousePos.x() < w/2) {
            x += dx;
            w -= dx;
        }
        if(d.iMousePos.x() > w/2) {
            w += dx;
        }
        if(d.iMousePos.y() < h/2) {
            y += dy;
            h -= dy;
        }
        if(d.iMousePos.y() > h/2) {
            h += dy;
        }
        move(x, y);
        resize(w, h);
        break;
    }

    repaint();
}

void CCWidgetRenderer::mouseDoubleClickEvent(QMouseEvent *)
{
    DPTR_D(CCWidgetRenderer);
    if (d.action == GestureMove)
        d.action = GestureResize;
    else
        d.action = GestureMove;
}

void CCWidgetRenderer::paintEvent(QPaintEvent *)
{
    DPTR_D(CCWidgetRenderer);

	if (!d.scale_in) {
		d.mutex_.lock();
	}

    QPainter p(this);
    if (!d.image.isNull()) {
        if (d.image.size() == QSize(d.width, d.height))
            p.drawImage(QPoint(), d.image);
        else
            p.drawImage(rect(), d.image);
    } else if (!d.preview.isNull()){
        if (d.preview.size() == QSize(d.width, d.height))
            p.drawImage(QPoint(), d.preview);
        else
            p.drawImage(rect(), d.preview);
    } else {
		d.preview = *&QImage(video_sizes(), QImage::Format_RGB32);
        d.preview.fill(QColor(Qt::black));
        p.drawImage(QPoint(), d.preview);
    }

	if (!d.scale_in) {
		d.mutex_.unlock();
	}
}

void CCWidgetRenderer::dragEnterEvent(QDragEnterEvent *)
{
}

void CCWidgetRenderer::dropEvent(QDropEvent *)
{
}

} //namespace CCAV
