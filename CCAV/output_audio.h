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

#ifndef CCAV_AUDIOOUTPUT_H
#define CCAV_AUDIOOUTPUT_H

#include <CCAV/master_output.h>
#include "master_array.h"
namespace CCAV {

class CCOutputAudioPrivate;
class Q_EXPORT CCOutputAudio : public CCOutput
{
    DPTR_DECLARE_PRIVATE(CCOutputAudio)
public:
    CCOutputAudio();
    virtual ~CCOutputAudio() = 0;

    void set_sample_rate(int rate);
    int sample_rate() const;

    void set_channels(int channels);
    int channels() const;

    void set_volume(double volume);
    double volume() const;
    void set_mute(bool yes);
    bool is_mute() const;

protected:
    CCOutputAudio(CCOutputAudioPrivate& d);
	virtual void convert_data(const ByteArray &data);
};

} //namespace CCAV
#endif // CCAV_AUDIOOUTPUT_H
