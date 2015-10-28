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

#ifndef CCAV_AVTHREAD_H
#define CCAV_AVTHREAD_H

#include <CCAV/master_packet.h>
namespace CCAV {

class CCDevoder;
class CCThreadPrivate;
class CCOutput;
class CCClock;
class Q_EXPORT CCThread 
{
    DPTR_DECLARE_PRIVATE(CCThread)
public:
	explicit CCThread();
    virtual ~CCThread();

    void set_clock(CCClock *clock);
    CCClock* clock() const;

    packet_queue* packet_queues() const;

    void set_decoder(CCDevoder *decoder);
    CCDevoder *decoder() const;

    void set_output(CCOutput *out);
    CCOutput* output() const;

    void set_demux_ended(bool ended);

    bool is_paused() const;

    virtual void stop();
    void pause(bool p); 

protected:
    CCThread(CCThreadPrivate& d);
    void reset_state();
 
    bool try_pause();

    DPTR_DECLARE(CCThread)
};
}

#endif // CCAV_AVTHREAD_H
