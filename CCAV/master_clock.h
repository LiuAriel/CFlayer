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

#ifndef CCAV_AVCLOCK_H
#define CCAV_AVCLOCK_H

#include <CCAV/master_global.h>
/*
 * CCClock is created by CCPlayer. The only way to access CCClock is through CCPlayer::master_clocks()
 * The default clock type is Audio's clock, i.e. vedio synchronizes to audio. If audio stream is not
 * detected, then the clock will set to External clock automatically.
 * I name it ExternalClock because the clock can be corrected outside, though it is a clock inside CCClock
 */
namespace CCAV {

static const double kThousandth = 0.001;

class Q_EXPORT CCClock
{
   
public:
    typedef enum {
        AudioClock, ExternalClock
    } ClockType;

    CCClock(ClockType c);
    CCClock();

    void set_clock_type(ClockType ct);
    ClockType clock_types() const;
    void set_clock_auto(bool a);
    bool is_clock_auto() const;
	bool is_active() const;
    inline double pts() const;
    inline double value() const; 
    inline void update_value(double pts);
    inline void update_video_pts(double pts);
    inline double video_pts() const;
    inline double delay() const; 
    inline void update_delay(double delay);

	void update_external_clock(long long msecs);
    void pause(bool p);
    void reset();
	
private:
    bool auto_clock;
    ClockType clock_type;
    mutable double pts_;
    double pts_v;
    double delay_;
};

//auto lambda = [](){return 0.05; };
double CCClock::value() const
{
    if (clock_type == AudioClock) {
        return pts_ + delay_;
	}
	else
	{
		return 0.05;
	}
}

void CCClock::update_value(double pts)
{
	if (clock_type == AudioClock)
	{
		pts_ = pts;
	}
}

void CCClock::update_video_pts(double pts)
{
    pts_v = pts;
}

double CCClock::video_pts() const
{
    return pts_v;
}

double CCClock::delay() const
{
    return delay_;
}

void CCClock::update_delay(double delay)
{
    delay_ = delay;
}

} //namespace CCAV
#endif // CCAV_AVCLOCK_H
