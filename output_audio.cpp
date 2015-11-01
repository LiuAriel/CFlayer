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

#include <CCAV/output_audio.h>
#include <private/output_audio_private.h>
#include "master_array.h"
namespace CCAV {
CCOutputAudio::CCOutputAudio()
    :CCOutput(*new CCOutputAudioPrivate())
{
}

CCOutputAudio::CCOutputAudio(CCOutputAudioPrivate &d)
    :CCOutput(d)
{
}

CCOutputAudio::~CCOutputAudio()
{
}

void CCOutputAudio::set_sample_rate(int rate)
{
    d_func().sample_rate = rate;
}

int CCOutputAudio::sample_rate() const
{
    return d_func().sample_rate;
}

void CCOutputAudio::set_channels(int channels)
{
    d_func().channels = channels;
}

int CCOutputAudio::channels() const
{
    return d_func().channels;
}

void CCOutputAudio::set_volume(double volume)
{
    DPTR_D(CCOutputAudio);
    d.vol = std::max<double>(volume, 0);
    d.mute = d.vol == 0;
}

double CCOutputAudio::volume() const
{
	return std::max<double>(d_func().vol, 0);
}

void CCOutputAudio::set_mute(bool yes)
{
    d_func().mute = yes;
}

bool CCOutputAudio::is_mute() const
{
	return !is_available() || d_func().mute;
}

void CCOutputAudio::convert_data(const ByteArray &data)
{
	d_func().data = data;
}

} //namespace CCAV
