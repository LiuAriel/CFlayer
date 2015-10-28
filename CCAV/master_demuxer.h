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

#ifndef _DEMUXER_H
#define _DEMUXER_H

#include <CCAV/master_global.h>
#include <qstring.h>
#include "master_array.h"
#include <condition_variable>
#include <memory>
#include <mutex>

struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVFrame;
struct AVStream;

namespace CCAV {

class CCClock;
class PacketData;
class Q_EXPORT CCDemuxer
{
public:
    CCDemuxer(const QString& file_name = QString());
    ~CCDemuxer();

    bool at_end() const;
    bool close();
    bool load_file(const QString& file_name);
    bool read_frame();
    PacketData* packet() const; 
    int stream() const; 

	void set_clock(CCClock *c);
	CCClock *clock() const;
    void seek(double q);
 
    void seek_forward();
    void seek_backward();

    AVFormatContext* format_contexts();
    void dump();
    std::string file_name() const;
    std::string audio_format_name() const;
    std::string audio_format_long_name() const;
    std::string video_format_name() const; 
    std::string video_format_long_name() const; 
    long long start_time() const; 
    long long duration() const;
    int bit_rate() const;
    double frame_rate() const; 
    long long frames() const; 
    bool is_input() const;
    int audio_streams() const;
    int video_streams() const;
    int subtitle_streams() const;

    int width() const; 
    int height() const; 
   
    AVCodecContext* audio_codec_context() const;
    AVCodecContext* video_codec_context() const;
    std::string audio_codec_name() const;
    std::string audio_codec_long_name() const;
    std::string video_codec_name() const;
    std::string video_codec_long_name() const;


    void started();
    void finished(); 
	bool started_;
private:
   // bool started_;
    bool eof;
    PacketData *pkt;
    long long ipts;
    int stream_idx;
	mutable /*int*/std::atomic<int> audio_stream, video_stream, subtitle_stream;

    bool find_codec();
    std::string format_name(AVFormatContext *ctx, bool longName = false) const;

    bool _is_input;
    AVFormatContext *format_context;
    AVCodecContext *a_codec_context, *v_codec_context;
    QString _file_name;
	CCClock *master_clock;
	std::condition_variable full_;
	std::condition_variable empty_;
	std::mutex mutex_;
};

} //namespace CCAV
#endif // _DEMUXER_H
