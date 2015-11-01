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
#include <CCAV/master_demuxer.h>
#include <CCAV/master_packet.h>
#include <CCAV/compat_common.h>
#include <thread>

namespace CCAV {

const long long kSeekInterval = 168; //ms
CCDemuxer::CCDemuxer(const QString& file_name)
    :started_(false),eof(false),pkt(new PacketData())
    ,ipts(0),stream_idx(-1),audio_stream(-2),video_stream(-2)
    ,subtitle_stream(-2),_is_input(true),format_context(0)
	,a_codec_context(0),v_codec_context(0),_file_name(file_name),master_clock(0)
{
    av_register_all();
    avformat_network_init();
    if (!_file_name.isEmpty())
        load_file(_file_name);
}

CCDemuxer::~CCDemuxer()
{
    close();
    if (pkt) {
        delete pkt;
        pkt = 0;
    }
    avformat_network_deinit();
}


bool CCDemuxer::read_frame()
{
	std::unique_lock<std::mutex> lock(mutex_);
   // AVPacket packet;
	std::unique_ptr<AVPacket, std::function<void(AVPacket*)>> packet(
		new AVPacket, [](AVPacket* p){ av_free_packet(p); delete p; });
    int ret = av_read_frame(format_context, packet.get()); 

    if (ret != 0) {
        if (ret == AVERROR_EOF) {
            if (!eof) {
                eof = true;
                started_ = false;
                qDebug("End of file. %s %d", __FUNCTION__, __LINE__);
            }
            return false;
        } else if (ret == AVERROR_INVALIDDATA) {
            qWarning("AVERROR_INVALIDDATA");
        }
        return false;
    }

    stream_idx = packet->stream_index; 
    if (!started_ && v_codec_context && v_codec_context->frame_number == 0) {
        started_ = true;  
    } else if (!started_ && a_codec_context && a_codec_context->frame_number == 0) {
        started_ = true;
    }
    if (stream_idx != video_streams() && stream_idx != audio_streams()) {
        qWarning("[CCDemuxer] unknown stream index: %d", stream_idx);
        return false;
    }
	
	pkt->date = ByteArray(packet->data, packet->size);
    pkt->duration = packet->duration;
    if (packet->dts != AV_NOPTS_VALUE) //has B-frames
        pkt->pts = packet->dts;
    else if (packet->pts != AV_NOPTS_VALUE)
        pkt->pts = packet->pts;
    else
        pkt->pts = 0;

    AVStream *stream = format_context->streams[stream_idx];
    pkt->pts *= av_q2d(stream->time_base);

    if (stream->codec->codec_type == AVMEDIA_TYPE_SUBTITLE
            && (packet->flags & AV_PKT_FLAG_KEY)
            &&  packet->convergence_duration != AV_NOPTS_VALUE)
        pkt->duration = packet->convergence_duration * av_q2d(stream->time_base);
    else if (packet->duration > 0)
        pkt->duration = packet->duration * av_q2d(stream->time_base);
    else
        pkt->duration = 0;

    //av_free_packet(&packet); 
    return true;
}

PacketData* CCDemuxer::packet() const
{
    return pkt;
}

int CCDemuxer::stream() const
{
    return stream_idx;
}

bool CCDemuxer::at_end() const
{
    return eof;
}

bool CCDemuxer::close()
{
    eof = false;
    stream_idx = -1;
    audio_stream = video_stream = subtitle_stream = -2;

    if (a_codec_context) {
        qDebug("closing a_codec_context");
        avcodec_close(a_codec_context);
        a_codec_context = 0;
    }

    if (v_codec_context) {
        qDebug("closing v_codec_context");
        avcodec_close(v_codec_context);
        v_codec_context = 0;
    }
 
    if (format_context) {
        qDebug("closing format_context");
        avformat_close_input(&format_context); 
        format_context = 0;
    }
    return true;
}

void CCDemuxer::set_clock(CCClock *c)
{
	master_clock = c;
}

CCClock* CCDemuxer::clock() const
{
	return master_clock;
}

void CCDemuxer::seek(double q)
{
    if ((!a_codec_context && !v_codec_context) || !format_context) {
        qWarning("can not seek. context not ready: %p %p %p", a_codec_context, v_codec_context, format_context);
        return;
    }
	std::unique_lock<std::mutex> lock(mutex_);
    q = std::max<double>(0.0, q);
    if (q >= 1.0) {
        qWarning("Invalid seek position %f/1.0", q);
        return;
    }

    
    int64_t t = int64_t(q*duration());///AV_TIME_BASE;
    
    if (t >= duration()) {
        qWarning("Invailid seek position: %lld/%lld", t, duration());
        return;
    }
    bool backward = t <= (int64_t)(pkt->pts*AV_TIME_BASE);
    qDebug("[CCDemuxer] seek to %f %f %lld / %lld backward=%d", q, pkt->pts, t, duration(), backward);
	
	int seek_flag =  /*(backward ? 0 :*/ AVSEEK_FLAG_BACKWARD/*)*/; //AVSEEK_FLAG_ANY
	int ret = av_seek_frame(format_context, -1, t, seek_flag);

    if (ret < 0) {
        return;
    }

	if (q == 0) {//rp
		qDebug("************seek to nullptr*********************. started = false");
		started_ = false;
		v_codec_context->frame_number = 0; 
	}

    if (master_clock) {
        master_clock->update_value(double(t)/double(AV_TIME_BASE));
        master_clock->update_external_clock(t/1000LL); 
    }
}

void CCDemuxer::seek_forward()
{
    if ((!a_codec_context && !v_codec_context) || !format_context) {
        qWarning("can not seek. context not ready: %p %p %p", a_codec_context, v_codec_context, format_context);
        return;
    }
    double pts = pkt->pts;
    if (master_clock) {
        pts = master_clock->value();
    } else {
        qWarning("[AVDemux] No master clock!");
    }
    double q = (double)((pts + 16)*AV_TIME_BASE)/(double)duration();
    seek(q);
}

void CCDemuxer::seek_backward()
{
    if ((!a_codec_context && !v_codec_context) || !format_context) {
        qWarning("can not seek. context not ready: %p %p %p", a_codec_context, v_codec_context, format_context);
        return;
    }
	double pts = pkt->pts;
    if (master_clock) {
        pts = master_clock->value();
    } else {
        qWarning("[AVDemux] No master clock!");
    }
    double q = (double)((pts - 16)*AV_TIME_BASE)/(double)duration();
    seek(q);
}

bool CCDemuxer::load_file(const QString &file_name)
{
    close();
    qDebug("all closed and reseted");
    _file_name = file_name;
    int ret = avformat_open_input(&format_context, qPrintable(_file_name), NULL, NULL);
    if (ret < 0) {
        return false;
    }
    format_context->flags |= AVFMT_FLAG_GENPTS;
   
    ret = avformat_find_stream_info(format_context, NULL);
    if (ret < 0) {
       
        return false;
    }

    find_codec();

    bool _has_audio = a_codec_context != 0;
    if (a_codec_context) {
        AVCodec *aCodec = avcodec_find_decoder(a_codec_context->codec_id);
        if (aCodec) {
            ret = avcodec_open2(a_codec_context, aCodec, NULL);
            if (ret < 0) {
                _has_audio = false;
          
            }
        } else {
            _has_audio = false;
            qDebug("Unsupported audio codec. id=%d.", a_codec_context->codec_id);
        }
    }
    if (master_clock->is_clock_auto()) {
        qDebug("auto select clock: audio > external");
        if (!_has_audio) {
            qWarning("No audio found or audio not supported. Using ExternalClock");
            master_clock->set_clock_type(CCClock::ExternalClock);
        } else {
            qDebug("Using AudioClock");
            master_clock->set_clock_type(CCClock::AudioClock);
        }
    }

    bool _has_vedio = v_codec_context != 0;
    if (v_codec_context) {
        AVCodec *vCodec = avcodec_find_decoder(v_codec_context->codec_id);
        if(!vCodec) {
            qWarning("Unsupported video codec. id=%d.", v_codec_context->codec_id);
            _has_vedio = false;
        }
        
        ret = avcodec_open2(v_codec_context, vCodec, NULL);
        if (ret < 0) {
            _has_vedio = false;
        } else {
            if (vCodec->capabilities & CODEC_CAP_DR1)
                v_codec_context->flags |= CODEC_FLAG_EMU_EDGE;
        }
        v_codec_context->skip_loop_filter = v_codec_context->skip_idct = AVDISCARD_DEFAULT;
        v_codec_context->flags2 &= ~CODEC_FLAG2_FAST;
        
        bool skipframes = false;
        v_codec_context->skip_frame = skipframes ? AVDISCARD_NONREF : AVDISCARD_DEFAULT;
    }
    started_ = false;
    return _has_audio || _has_vedio;
}

AVFormatContext* CCDemuxer::format_contexts()
{
    return format_context;
}

void CCDemuxer::dump()
{
	av_dump_format(format_context, 0, qPrintable(_file_name), false);
    fflush(0);
   /* qDebug("[AVFormatContext::duration = %lld]", duration());
    qDebug("video format: %s [%s]", video_format_name(), video_format_long_name());
    qDebug("Audio: %s [%s]", audio_codec_name(), audio_codec_long_name());*/
    if (a_codec_context)
        qDebug("sample rate: %d, channels: %d", a_codec_context->sample_rate, a_codec_context->channels);
    struct stream_info {
        int index;
        AVCodecContext* ctx;
        const char* name;
    };

    stream_info stream_infos[] = {
          {audio_streams(),    a_codec_context, "audio stream"}
        , {video_streams(),    v_codec_context, "video_stream"}
        , {0,                0,               0}
    };
    for (int idx = 0; stream_infos[idx].name != 0; ++idx) {
        qDebug("%s: %d", stream_infos[idx].name, stream_infos[idx].index);
        if (stream_infos[idx].index < 0) {
            qDebug("stream not available");
            continue;
        }
        AVStream *stream = format_context->streams[idx];
        if (!stream)
            continue;
        qDebug("[AVStream::start_time = %lld]", stream->start_time);
        AVCodecContext *ctx = stream_infos[idx].ctx;
        if (ctx) {
            qDebug("[AVCodecContext::time_base = %d, %d, %.2f %.2f]", ctx->time_base.num, ctx->time_base.den
                ,1.0 * ctx->time_base.num / (1 + ctx->time_base.den)
                ,1.0 / (1.0 * ctx->time_base.num / (1 + ctx->time_base.den))
                );
        }
        qDebug("[AVStream::avg_frame_rate = %d, %d, %.2f]", stream->avg_frame_rate.num, stream->avg_frame_rate.den
                ,1.0 * stream->avg_frame_rate.num / stream->avg_frame_rate.den);
        qDebug("[AVStream::r_frame_rate = %d, %d, %.2f]", stream->r_frame_rate.num, stream->r_frame_rate.den
                ,1.0 * stream->r_frame_rate.num / stream->r_frame_rate.den);
        qDebug("[AVStream::time_base = %d, %d, %.2f]", stream->time_base.num, stream->time_base.den
                ,1.0 * stream->time_base.num / stream->time_base.den);
    }

}

std::string CCDemuxer::file_name() const
{
    return format_context->filename;
}

std::string CCDemuxer::video_format_name() const
{
    return format_name(format_context, false);
}

std::string CCDemuxer::video_format_long_name() const
{
    return format_name(format_context, true);
}

long long CCDemuxer::start_time() const
{
    return format_context->start_time;
}

long long CCDemuxer::duration() const
{
    return format_context->duration;
}

int CCDemuxer::bit_rate() const
{
    return format_context->bit_rate;
}

double CCDemuxer::frame_rate() const
{
    AVStream *stream = format_context->streams[video_streams()];
    return (double)stream->r_frame_rate.num / (double)stream->r_frame_rate.den;
}

long long CCDemuxer::frames() const
{
    return format_context->streams[video_streams()]->nb_frames; 
}

bool CCDemuxer::is_input() const
{
    return _is_input;
}

int CCDemuxer::audio_streams() const
{
    if (audio_stream != -2) 
        return audio_stream;
    audio_stream = -1;
    for(unsigned int i=0; i<format_context->nb_streams; ++i) {
        if(format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream = i;
            break;
        }
    }
    return audio_stream;
}

int CCDemuxer::video_streams() const
{
    if (video_stream != -2) 
        return video_stream;
    video_stream = -1;
    for(unsigned int i=0; i<format_context->nb_streams; ++i) {
        if(format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = i;
            break;
        }
    }
    return video_stream;
}

int CCDemuxer::subtitle_streams() const
{
    if (subtitle_stream != -2) 
        return subtitle_stream;
    subtitle_stream = -1;
    for(unsigned int i=0; i<format_context->nb_streams; ++i) {
        if(format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            subtitle_stream = i;
            break;
        }
    }
    return subtitle_stream;
}

int CCDemuxer::width() const
{
    return video_codec_context()->width;
}

int CCDemuxer::height() const
{
    return video_codec_context()->height;
}


//codec
AVCodecContext* CCDemuxer::audio_codec_context() const
{
    return a_codec_context;
}

AVCodecContext* CCDemuxer::video_codec_context() const
{
    return v_codec_context;
}

std::string CCDemuxer::audio_codec_name() const
{
    if (a_codec_context)
        return a_codec_context->codec->name;
    return "";
}

std::string CCDemuxer::audio_codec_long_name() const
{
    if (a_codec_context)
        return a_codec_context->codec->long_name;
    return "";
}

std::string CCDemuxer::video_codec_name() const
{
    if (v_codec_context)
        return v_codec_context->codec->name;
    return "";
}

std::string CCDemuxer::video_codec_long_name() const
{
    if (v_codec_context)
        return v_codec_context->codec->long_name;
    return "";
}


bool CCDemuxer::find_codec()
{
    if (video_stream != -2 && audio_stream != -2 && subtitle_stream != -2)
        return (video_stream != -1) && (audio_stream != -1) && (subtitle_stream != -1);
    video_stream = audio_stream = subtitle_stream = -1;
    AVMediaType type = AVMEDIA_TYPE_UNKNOWN;
    for (unsigned int i=0; i<format_context->nb_streams; ++i) {
        type = format_context->streams[i]->codec->codec_type;
        if (type == AVMEDIA_TYPE_VIDEO && video_stream < 0) {
            video_stream = i;
            v_codec_context = format_context->streams[video_stream]->codec;
            if (v_codec_context->codec_id == CODEC_ID_H264) {
                v_codec_context->thread_type = FF_THREAD_FRAME; //FF_THREAD_SLICE;
                v_codec_context->thread_count = std::thread::hardware_concurrency();
            } else {
               
            }
        } else if (type == AVMEDIA_TYPE_AUDIO && audio_stream < 0) {
            audio_stream = i;
            a_codec_context = format_context->streams[audio_stream]->codec;
        } else if (type == AVMEDIA_TYPE_SUBTITLE && subtitle_stream < 0) {
            subtitle_stream = i;
        }
        if (audio_stream >=0 && video_stream >= 0 && subtitle_stream >= 0)
            return true;
    }
    return false;
}

std::string CCDemuxer::format_name(AVFormatContext *ctx, bool longName) const
{
    if (is_input())
        return longName ? ctx->iformat->long_name : ctx->iformat->name;
    else
        return longName ? ctx->oformat->long_name : ctx->oformat->name;
}

} //namespace CCAV
