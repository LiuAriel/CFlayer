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

#ifndef CCAV_WIDGETRENDERER_H
#define CCAV_WIDGETRENDERER_H

#include <CCAV/image_renderer.h>
#include <qwidget.h>
#include "master_array.h"
namespace CCAV {

class CCWidgetRendererPrivate;
class Q_EXPORT CCWidgetRenderer : public QWidget, public CCImageRenderer
{
    Q_OBJECT
    DPTR_DECLARE_PRIVATE(CCWidgetRenderer)
public:
   
    enum GestureAction { GestureMove, GestureResize};

    explicit CCWidgetRenderer(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~CCWidgetRenderer();

    //virtual void registerEventFilter(EventFilter *filter);
	virtual bool write() override;
    void set_preview(const QImage& preivew);

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
#if !CONFIG_EZX
    virtual void paintEvent(QPaintEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dropEvent(QDropEvent *);
#endif
    
protected:
    CCWidgetRenderer(CCWidgetRendererPrivate& d, QWidget *parent, Qt::WindowFlags f);
};

} //namespace CCAV
#endif // CCAV_WIDGETRENDERER_H
