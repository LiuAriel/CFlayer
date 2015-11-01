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

#ifndef CCAV_AVTHREAD_P_H
#define CCAV_AVTHREAD_P_H

#include <CCAV/master_packet.h>
#include <CCAV/master_decode.h>
#include <CCAV/master_output.h>
#include <CCAV/master_global.h>
#include <condition_variable>
#include <memory>
#include <mutex>
namespace CCAV {

class CCDevoder;
class PacketData;
class CCClock;
class Q_EXPORT CCThreadPrivate : public DPtrPrivate<CCThread>
{
public:
    CCThreadPrivate():paused(false),demux_end(false),stop(false),clock(0)
      ,dec(0),writer(0) {
    }
    virtual ~CCThreadPrivate() {}

    bool paused;
    bool demux_end;
	volatile std::atomic<bool> stop;
    CCClock *clock;
    packet_queue packets;
    CCDevoder *dec;
    CCOutput *writer;
	std::condition_variable full_;
	std::condition_variable empty_;
	std::mutex mutex_;
};

} //namespace CCAV
#endif // CCAV_AVTHREAD_P_H
