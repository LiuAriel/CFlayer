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

#ifndef _DEMUXTHREAD_H
#define _DEMUXTHREAD_H

#include <CCAV/master_global.h>
#include <condition_variable>
#include <memory>
#include <mutex>
namespace CCAV {

class CCDemuxer;
class CCThread;
class Q_EXPORT CCDemuxerThread
{
public:
    explicit CCDemuxerThread();
    explicit CCDemuxerThread(CCDemuxer *dmx);
    void set_demuxer(CCDemuxer *dmx);
    void set_audio_thread(CCThread *thread);
    void set_video_thread(CCThread *thread);
    void seek(double pos);
    void seek_forward();
    void seek_backward();
    bool is_paused() const;
	void run_demuxer();
	void runDemuxer();
    void stop();
    void pause(bool p);
	void runEnd();
protected:
    bool try_pause();

private:
    bool paused;
    volatile bool end, end_;
    CCDemuxer *demuxer;
    CCThread *audio_thread, *video_thread;
    int audio_stream, video_stream;
	std::condition_variable full_;
	std::condition_variable empty_;
	std::mutex mutex_;
	bool is_bool_;
};

} //namespace CCAV
#endif // _DEMUXTHREAD_H
