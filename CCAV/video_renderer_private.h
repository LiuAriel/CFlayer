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

#ifndef CCAV_VIDEORENDERER_P_H
#define CCAV_VIDEORENDERER_P_H

#include <private/master_output_private.h>
#include <CCAV/compat_common.h>
#include <mutex>

namespace CCAV {

class EventFilter;
class Q_EXPORT CCVideoRendererPrivate : public CCOutputPrivate
{
public:
	CCVideoRendererPrivate() :scale_in(true),width(480), height(320), src_width(0)
      ,src_height(0),event_filter(0) {
    }
    virtual ~CCVideoRendererPrivate(){}
    int width, height;
    int src_width, src_height;
    EventFilter *event_filter;
	std::mutex mutex_;
	bool scale_in;
};

} //namespace CCAV
#endif // CCAV_VIDEORENDERER_P_H
