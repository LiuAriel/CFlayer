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


#ifndef CCAV_AUDIOOUTPUT_P_H
#define CCAV_AUDIOOUTPUT_P_H

#include <private/master_output_private.h>
#include "master_array.h"
namespace CCAV {

class Q_EXPORT CCOutputAudioPrivate : public CCOutputPrivate
{
public:
    CCOutputAudioPrivate():mute(false),channels(2)
      ,vol(1),sample_rate(44100)
    {
    }
    virtual ~CCOutputAudioPrivate(){}
    bool mute;
    int channels;
    double vol;
    int sample_rate;
	ByteArray data;
};

} //namespace CCAV
#endif // CCAV_AUDIOOUTPUT_P_H
