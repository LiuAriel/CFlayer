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

#ifndef CCAV_VIDEORENDERER_H
#define CCAV_VIDEORENDERER_H

#include <QtCore/QSize>
#include <CCAV/master_output.h>

struct AVCodecContext;
struct AVFrame;
class QImage;
namespace CCAV {

class EventFilter;
class CCVideoRendererPrivate;
class Q_EXPORT CCVideoRenderer : public CCOutput
{
    DPTR_DECLARE_PRIVATE(CCVideoRenderer)
public:
    CCVideoRenderer();
    virtual ~CCVideoRenderer() = 0;

	void scale_in(bool q);
	bool scale_in() const;	
	void set_source_size(const QSize& s); 
	void set_source_size(int width, int height); 
	QSize last_size() const;
	int last_width() const;
	int last_height() const;

    virtual void registerEventFilter(EventFilter* filter);
    virtual bool open() override;
    virtual bool close() override;
    virtual QImage current_frame_image() const = 0; 
    void resize_video(const QSize& size);
    void resize_video(int width, int height);
    QSize video_sizes() const;
    int video_widths() const;
    int video_heights() const;

protected:
    CCVideoRenderer(CCVideoRendererPrivate &d);
};

} //namespace CCAV
#endif // CCAV_VIDEORENDERER_H
