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


#ifndef CCAV_WIDGETRENDERER_P_H
#define CCAV_WIDGETRENDERER_P_H

#include <private/image_renderer_private.h>
#include <CCAV/widget_renderer.h>

namespace CCAV {

class Q_EXPORT CCWidgetRendererPrivate : public CCImageRendererPrivate
{
public:
    CCWidgetRendererPrivate():action(CCWidgetRenderer::GestureMove) {}
    virtual ~CCWidgetRendererPrivate(){}
    QPoint iMousePos, gMousePos;
    CCWidgetRenderer::GestureAction action;
};

} //namespace CCAV
#endif // CCAV_WIDGETRENDERER_P_H
