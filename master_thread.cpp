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

#include <CCAV/master_thread.h>
#include <private/master_thread_private.h>

namespace CCAV {
CCThread::CCThread()
{
}

CCThread::CCThread(CCThreadPrivate &d)
    :DPTR_INIT(&d)
{
}

CCThread::~CCThread()
{
   
}

bool CCThread::is_paused() const
{
    return d_func().paused;
}

void CCThread::stop()
{
    pause(false);
    DPTR_D(CCThread);
    if (d.writer)
        d.writer->pause(false); 
    d.stop = true;
    d.packets.set_blocking(false); 
    d.packets.clear();
}

void CCThread::pause(bool p)
{
    DPTR_D(CCThread);
    if (d.paused == p)
        return;
    d.paused = p;
    if (!d.paused) {
        qDebug("wake up paused thread");
		d.full_.notify_all();
    }
}

void CCThread::set_clock(CCClock *clock)
{
    d_func().clock = clock;
}

CCClock* CCThread::clock() const
{
    return d_func().clock;
}

packet_queue* CCThread::packet_queues() const
{
    return const_cast<packet_queue*>(&d_func().packets);
}

void CCThread::set_decoder(CCDevoder *decoder)
{
    d_func().dec = decoder;
}

CCDevoder* CCThread::decoder() const
{
    return d_func().dec;
}

void CCThread::set_output(CCOutput *out)
{
    d_func().writer = out;
}

CCOutput* CCThread::output() const
{
    return d_func().writer;
}

void CCThread::set_demux_ended(bool ended)
{
    d_func().demux_end = ended;
}

void CCThread::reset_state()
{
    DPTR_D(CCThread);
    pause(false);
    if (d.writer)
        d.writer->pause(false); 
    d.stop = false;
    d.demux_end = false;
    d.packets.set_blocking(true);
    d.packets.clear();
}

bool CCThread::try_pause()
{
    DPTR_D(CCThread);
    if (!d.paused)
        return false;
	std::unique_lock<std::mutex> lock(d.mutex_);
	d.full_.wait(lock);
    qDebug("paused thread waked up!!!");
    return true;
}

} //namespace CCAV
