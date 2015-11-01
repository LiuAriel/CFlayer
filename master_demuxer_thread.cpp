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

#include <CCAV/master_demuxer_thread.h>
#include <CCAV/master_demuxer.h>
#include <CCAV/master_decode.h>
#include <CCAV/master_packet.h>
#include <CCAV/master_thread.h>
#include <thread>
namespace CCAV {

CCDemuxerThread::CCDemuxerThread() :
paused(false), end(false), demuxer(0), audio_thread(0), video_thread(0), end_(false), is_bool_(false)
{
}

CCDemuxerThread::CCDemuxerThread(CCDemuxer *dmx) :
end(false), audio_thread(0), video_thread(0), end_(end), is_bool_(false)
{
    set_demuxer(dmx);
}

void CCDemuxerThread::set_demuxer(CCDemuxer *dmx)
{
    demuxer = dmx;
}

void CCDemuxerThread::set_audio_thread(CCThread *thread)
{
    if (audio_thread) {
        audio_thread->stop();
        delete audio_thread;
        audio_thread = 0;
    }
    audio_thread = thread;
}

void CCDemuxerThread::set_video_thread(CCThread *thread)
{
    if (video_thread) {
        video_thread->stop();
        delete video_thread;
        video_thread = 0;
    }
    video_thread = thread;
}

void CCDemuxerThread::seek(double pos)
{
	
	mutex_.lock();
    audio_thread->packet_queues()->clear();
    video_thread->packet_queues()->clear();
    demuxer->seek(pos);
	mutex_.unlock();
    if (is_paused()) {
        pause(false);
        video_thread->pause(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
        pause(true);
        video_thread->pause(true);
    }
}

void CCDemuxerThread::seek_forward()
{
	//std::unique_lock<std::mutex> lock(mutex_);
	//if (!mutex_.try_lock()) {
	//	mutex_.unlock(); //may be still blocking in demux thread
	//	mutex_.lock();
	//}
	mutex_.lock();
    audio_thread->packet_queues()->clear();
    video_thread->packet_queues()->clear();
    demuxer->seek_forward();
	mutex_.unlock();
    if (is_paused()) {
        pause(false);
        video_thread->pause(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
        pause(true);
        video_thread->pause(true);
    }
}

void CCDemuxerThread::seek_backward()
{
	mutex_.lock();
    audio_thread->packet_queues()->clear();
    video_thread->packet_queues()->clear();
    demuxer->seek_backward();
	mutex_.unlock();
    if (is_paused()) {
        pause(false);
        video_thread->pause(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
        pause(true);
        video_thread->pause(true);
    }
}

bool CCDemuxerThread::is_paused() const
{
    return paused;
}

void CCDemuxerThread::stop()
{
	end_ = true;
    end = true;
    //this will not affect the pause state if we pause the output
    audio_thread->set_demux_ended(true);
    video_thread->set_demux_ended(true);
    audio_thread->packet_queues()->block_full(false); //??
    video_thread->packet_queues()->block_full(false); //?
    pause(false);
}

void CCDemuxerThread::pause(bool p)
{
    if (paused == p)
        return;
    paused = p;
	if (!paused)
		full_.notify_all();
}

void CCDemuxerThread::run_end()
{
	end_ = false;
	while (!end_)
	{
		if (demuxer->started_ == false)
		{
			end_ = true;
			stop();
		}
	}
	qDebug("DemuxSTOP thread stops running....");
}

void CCDemuxerThread::run_demuxer()
{
	end = false;

	audio_stream = demuxer->audio_streams();
	video_stream = demuxer->video_streams();

	int index = 0;
	PacketData pkt;
	end = false;
	pause(false);
	packet_queue *aqueue = audio_thread->packet_queues();
	packet_queue *vqueue = video_thread->packet_queues();
	aqueue->set_blocking(true);
	vqueue->set_blocking(true);
	bool _has_audio = audio_thread->decoder()->is_available();
	bool _has_video = video_thread->decoder()->is_available();
	demuxer->started_ = true;
	while (!end) {
		if (try_pause())
			continue; 

		mutex_.lock();
		if (!demuxer->read_frame()) {
			mutex_.unlock();
			continue;
		}
		mutex_.unlock();
		index = demuxer->stream();
		pkt = *demuxer->packet(); 
		if (index == audio_stream) {
			if (_has_video)
				aqueue->block_full(vqueue->size() >= vqueue->threshold());
			aqueue->put(std::move(pkt)); //affect video_thread
		}
		else if (index == video_stream) {
			if (_has_audio)
				vqueue->block_full(aqueue->size() >= aqueue->threshold());
			vqueue->put(std::move(pkt)); //affect audio_thread
		}
		else { //subtitle
			continue;
		}
	
	}
	aqueue->put(PacketData());
	vqueue->put(PacketData());
	qDebug("Demux thread stops running....");
}

bool CCDemuxerThread::try_pause()
{
    if (!paused)
        return false;
	std::unique_lock<std::mutex> lock(mutex_);
	full_.wait(lock);
    return true;
}


} //namespace CCAV
