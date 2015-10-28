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


#include <CCAV/master_clock.h>

namespace CCAV {

CCClock::CCClock(CCClock::ClockType c)
    :auto_clock(true),clock_type(c)
{
    pts_ = pts_v = delay_ = 0;
}

CCClock::CCClock()
    :auto_clock(true),clock_type(AudioClock)
{
    pts_ = pts_v = delay_ = 0;
}

void CCClock::set_clock_type(ClockType ct)
{
    clock_type = ct;
}

CCClock::ClockType CCClock::clock_types() const
{
    return clock_type;
}

void CCClock::set_clock_auto(bool a)
{
    auto_clock = a;
}

bool CCClock::is_clock_auto() const
{
    return auto_clock;
}

void CCClock::update_external_clock(long long msecs)
{
    if (clock_type != ExternalClock)
        return;
    qDebug("External clock change: %f ==> %f", value(), double(msecs) * kThousandth);
    pts_ = double(msecs) * kThousandth; 
}

void CCClock::pause(bool p)
{

}

void CCClock::reset()
{
    pts_ = pts_v = delay_ = 0;
}


} //namespace CCAV
