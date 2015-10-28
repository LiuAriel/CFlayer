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

#ifndef CCAV_WRITER_H
#define CCAV_WRITER_H

#include <CCAV/master_global.h>
#include "master_array.h"
namespace CCAV {

class CCDevoder;
class CCOutputPrivate;
class Q_EXPORT CCOutput
{
    DPTR_DECLARE_PRIVATE(CCOutput)
public:
    CCOutput();
    virtual ~CCOutput() = 0;
	bool write_data(const ByteArray& data);
    bool is_available() const;
    virtual bool open() = 0;
    virtual bool close() = 0;
    void pause(bool p); 
    bool is_paused() const;
protected:
    CCOutput(CCOutputPrivate& d);
	virtual void convert_data(const ByteArray& data) = 0;
	virtual bool write() = 0;
	bool try_pause();

    DPTR_DECLARE(CCOutput)
};

} //namespace CCAV
#endif //CCAV_WRITER_H
