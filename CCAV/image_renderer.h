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

#ifndef CCAV_IMAGERENDERER_H
#define CCAV_IMAGERENDERER_H

#include <CCAV/video_renderer.h>
#include <qimage.h>
#include "master_array.h"

namespace CCAV {

class CCImageRendererPrivate;
class Q_EXPORT CCImageRenderer : public CCVideoRenderer
{
    DPTR_DECLARE_PRIVATE(CCImageRenderer)
public:
    CCImageRenderer();
    virtual ~CCImageRenderer();
	virtual QImage current_frame_image() const override;
	virtual void convert_data(const ByteArray &data) override;
    void set_preview(const QImage& preview);
    QImage preview_image() const;
    QImage current_image() const;
protected:
    CCImageRenderer(CCImageRendererPrivate& d);
};

} //namespace CCAV
#endif // CCAV_IMAGERENDERER_H
