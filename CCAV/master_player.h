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

#ifndef CCAV_AVPLAYER_H
#define CCAV_AVPLAYER_H

#include <QtCore/QObject>
#include <QtCore/QSize>
#include <CCAV/master_clock.h>
#include <CCAV/master_demuxer.h>
#include <util.h>
#include <functional>

namespace CCAV {
class CCOutputAudio;
class CCAudioThread;
class CCVideoThread;
class CCDecodeAudio;
class CCDecodeVideo;
class CCVideoRenderer;
class CCClock;
class CCDemuxerThread;
class EventFilter;
class Q_EXPORT CCPlayer : public QObject
{
    Q_OBJECT
public:
    explicit CCPlayer(QObject *parent = 0);
    ~CCPlayer();

   
    CCClock* master_clocks();
    void set_files(const QString& path);
	QString file() const;
   
    void set_capture_name(const std::string& name);
    void set_capture_savedir(const std::string& dir); 
    bool capture_video();
    bool play(const QString& path);
    void pauses(bool p);
    bool is_paused() const;
    void set_renderers(CCVideoRenderer* renderer);

    void set_mute(bool mute);
    bool is_mute() const;
	int  tests(int a);
	void decode_video();
	void decode_audio();
	void demultiplex();
	void stop_datas();
	bool is_plays() const;
	void demultiplexstop();
	void set_volume(double vol);
	double  volume() const;
public slots:
    void plays(); //replay
    void play_next_frame();
    void seek_forward();
    void seek_backward();

protected slots:
    void resize_video(const QSize& size);

protected:
	int avTimerId;
    AVFormatContext	*formatCtx; 
    AVCodecContext *aCodecCtx, *vCodecCtx; 
    QString path;
    std::string capture_name, capture_dir;
    int m_drop_count;
    CCDemuxer demuxer;
    CCDemuxerThread *demuxer_thread;
    CCClock *clock;
    CCVideoRenderer *renderer; //list?
    CCOutputAudio *audio;
    CCDecodeAudio *audio_dec;
    CCDecodeVideo *video_dec;
    CCAudioThread *audio_thread;
    CCVideoThread *video_thread;

    friend class EventFilter;
    EventFilter *event_filter;

	//std::vector<std::thread> stages_;
	std::thread thread_video;
	std::thread thread_audio;
	std::thread thread_demuxer;
	std::thread thread_demuxer_stop;
	bool is_audio;
	bool is_video;
	bool is_demuxer;
	bool is_demuxer_stop;

	
public:
	std::function<void()> func_play;
	std::function<void()> func_play_next_frame;
	std::function<void()> func_stop_data;
	std::function<void()> func_seek_forward;
	std::function<void()> func_seek_backward;
	std::function<void(double)> func_seek;
	std::function<void(bool)> func_pauses;
	std::function<bool(QString)> func_open_file;
	std::function<void(double)> func_set_volume;
	std::function<double()> func_volume;
	std::function<void(bool)> func_set_mute;
};

} //namespace CCAV
#endif // CCAV_AVPLAYER_H
