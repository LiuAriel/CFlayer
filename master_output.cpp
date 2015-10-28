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

#include <CCAV/master_output.h>
#include <private/master_output_private.h>
namespace CCAV {

CCOutput::CCOutput()
{
}

CCOutput::CCOutput(CCOutputPrivate &d)
    :DPTR_INIT(&d)
{
}


CCOutput::~CCOutput()
{
    pause(false); 
}

bool CCOutput::write_data(const ByteArray &data)
{
	Q_UNUSED(data);
	convert_data(data);
	bool result = write();
	try_pause();
	return result;
}

bool CCOutput::is_available() const
{
    return d_func().available;
}

void CCOutput::pause(bool p)
{
    DPTR_D(CCOutput);
    if (d.paused == p)
        return;
    d.paused = p;
    if (!d.paused)
	   d.full_.notify_all();
}

bool CCOutput::is_paused() const
{
    return d_func().paused;
}

bool CCOutput::try_pause()
{
    DPTR_D(CCOutput);
    if (!d.paused)
        return false;
	std::unique_lock<std::mutex> lock(d.mutex_);
	d.full_.wait(lock);
    return true;
}

} //namespace CCAV
