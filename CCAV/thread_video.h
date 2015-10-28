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


#ifndef CCAV_VIDEOTHREAD_H
#define CCAV_VIDEOTHREAD_H

#include <CCAV/master_thread.h>
#include <QtCore/QSize>

namespace CCAV {

class CCImageConverter;
class CCVideoThreadPrivate;
class CCVideoThread : public CCThread
{
    DPTR_DECLARE_PRIVATE(CCVideoThread)
public:
    explicit CCVideoThread();
    CCImageConverter* set_image_converter(CCImageConverter *converter);
    CCImageConverter* image_converter() const;
    QSize current_rawimage_size() const; 
    double current_pts() const;
	void run_video();
protected:
};

} //namespace CCAV
#endif // CCAV_VIDEOTHREAD_H
