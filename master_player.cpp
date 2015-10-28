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

#include <CCAV/master_player.h>

#include <qevent.h>
#include <qpainter.h>
#include <QApplication>
#include <QEvent>
#include <QtCore/QDir>
#include <assert.h>
#include <CCAV/master_demuxer.h>
#include <CCAV/thread_audio.h>
#include <CCAV/master_packet.h>
#include <CCAV/decode_audio.h>
#include <CCAV/video_renderer.h>
#include <CCAV/master_clock.h>
#include <CCAV/decode_video.h>
#include <CCAV/widget_renderer.h>
#include <CCAV/thread_video.h>
#include <CCAV/master_demuxer_thread.h>
#include <CCAV/master_event_filter.h>
#if HAVE_PORTAUDIO
#include <CCAV/port_audio.h>
#endif //HAVE_PORTAUDIO
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#ifdef __cplusplus
}
#endif //__cplusplus

using std::thread;

namespace CCAV {

CCPlayer::CCPlayer(QObject *parent) :
	QObject(parent),capture_dir("capture"),renderer(0),audio(0)
	, event_filter(0), is_audio(false), is_video(false), is_demuxer(false), is_demuxer_stop(false)
{
    qDebug("CCVA %s\nCopyright (C) 2015 Juno<junowendy@gmail.com>"
           "\nDistributed under GPLv3 or later"
           "\nShenzhen, China"
           , CCAV_VERSION_STR_LONG);
    
    clock = new CCClock(CCClock::AudioClock);
	demuxer.set_clock(clock);
    audio = new CCPortAudio();
    audio_dec = new CCDecodeAudio();
    audio_thread = new CCAudioThread();
    audio_thread->set_clock(clock);
    audio_thread->set_decoder(audio_dec);
    audio_thread->set_output(audio);

    video_dec = new CCDecodeVideo();

    video_thread = new CCVideoThread();
    video_thread->set_clock(clock);
    video_thread->set_decoder(video_dec);

    demuxer_thread = new CCDemuxerThread();
    demuxer_thread->set_demuxer(&demuxer);
    demuxer_thread->set_audio_thread(audio_thread);
    demuxer_thread->set_video_thread(video_thread);

    event_filter = new EventFilter(this);
}

CCPlayer::~CCPlayer()
{
	stop_datas();
	
    if (audio) {
        delete audio;
        audio = 0;
    }
    if (audio_dec) {
        delete audio_dec;
        audio_dec = 0;
    }
    if (video_dec) {
        delete video_dec;
        video_dec = 0;
    }
}

CCClock* CCPlayer::master_clocks()
{
    return clock;
}

void CCPlayer::set_renderers(CCVideoRenderer *r)
{
    if (renderer) {
		if (is_plays())
			stop_datas();
    }
    renderer = r;
    renderer->registerEventFilter(event_filter);
    video_thread->set_output(renderer);
    renderer->resize_video(renderer->video_sizes()); 
}

void CCPlayer::set_mute(bool mute)
{
    if (audio)
        audio->set_mute(mute);
}

bool CCPlayer::is_mute() const
{
    return audio && audio->is_mute();
}

void CCPlayer::resize_video(const QSize &size)
{
    renderer->resize_video(size);
}

void CCPlayer::set_files(const QString &path)
{
    this->path = path;
}

QString CCPlayer::file() const
{
    return path;
}

void CCPlayer::set_capture_name(const std::string &name)
{
    capture_name = name;
}

void CCPlayer::set_capture_savedir(const std::string &dir)
{
    capture_dir = dir;
}

bool CCPlayer::capture_video()
{
    return true;
}

bool CCPlayer::play(const QString& path)
{
	set_files(path);
	plays();
	return true;
}

bool CCPlayer::is_plays() const 
{
	return is_demuxer == true || is_video == true || is_audio == true || is_demuxer_stop == true;
}

void CCPlayer::pauses(bool p)
{
    demuxer_thread->pause(p);
    audio_thread->pause(p);
    video_thread->pause(p);
}

bool CCPlayer::is_paused() const
{
    return demuxer_thread->is_paused() | audio_thread->is_paused() | video_thread->is_paused();
}

int CCPlayer::tests(int a)
{
	return a;
}

void CCPlayer::plays()
{
    if (path.isEmpty()) {
		qDebug("No file to play...");
		return;
	}
	if (is_plays())
	{
		stop_datas();
	}
    qDebug("loading: %s ...", path);
    if (!demuxer.load_file(path)) {
        return;
	}
    demuxer.dump();
	assert(clock != 0);
    clock->reset();
    formatCtx = demuxer.format_contexts();
    vCodecCtx = demuxer.video_codec_context();
    aCodecCtx = demuxer.audio_codec_context();
    if (audio && aCodecCtx) {
        audio->set_sample_rate(aCodecCtx->sample_rate);
        audio->set_channels(aCodecCtx->channels);
        if (!audio->open())
            return;
    }

    m_drop_count = 0;

    audio_dec->set_codec_context(aCodecCtx);
    if (aCodecCtx) {
        qDebug("Starting audio thread...");
		is_audio = true;
		thread_audio = std::thread(&CCPlayer::decode_audio, this);
    }

    video_dec->set_codec_context(vCodecCtx);
    if (vCodecCtx) {
        qDebug("Starting video thread...");
		is_video = true;
		thread_video = std::thread(&CCPlayer::decode_video, this);
    }

	is_demuxer = true;
	thread_demuxer_stop = std::thread(&CCPlayer::demultiplex, this);

	is_demuxer_stop = true;
	thread_demuxer = std::thread(&CCPlayer::demultiplexstop, this);
}

void CCPlayer::decode_audio()
{
	audio_thread->run_audio();
}

void CCPlayer::decode_video()
{
	video_thread->run_video();
}

void CCPlayer::demultiplex()
{
	demuxer_thread->runDemuxer();
}

void CCPlayer::demultiplexstop()
{
	demuxer_thread->runEnd();
}

void CCPlayer::stop_datas()
{
	demuxer_thread->stop();
	if (thread_demuxer.joinable()) {
		thread_demuxer.join();
	}

	audio_thread->stop();
	if (thread_audio.joinable()) {
		thread_audio.join();
	}
	
	video_thread->stop();
	if (thread_video.joinable()) {
		thread_video.join();
	}

	if (thread_demuxer_stop.joinable()) {
		thread_demuxer_stop.join();
	}
}

//FIXME: If not playing, it will just play but not play one frame.
void CCPlayer::play_next_frame()
{
	if (!is_plays())
	{
		plays();
	}
    pauses(false);
    pauses(true);
}

void CCPlayer::seek_forward()
{
    demuxer_thread->seek_forward();
}

void CCPlayer::seek_backward()
{
    demuxer_thread->seek_backward();
}

} //namespace CCAV
