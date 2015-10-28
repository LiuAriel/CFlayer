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


#ifndef CCAV_AOPORTAUDIO_H
#define CCAV_AOPORTAUDIO_H

#include <CCAV/output_audio.h>
#include "master_array.h"
namespace CCAV {

class CCPortAudioPrivate;
class Q_EXPORT CCPortAudio : public CCOutputAudio
{
    DPTR_DECLARE_PRIVATE(CCPortAudio)
public:
    CCPortAudio();
    virtual ~CCPortAudio();

	virtual bool open() override;
	virtual bool close() override;

protected:
	virtual bool write() override;
};

} //namespace CCAV
#endif // CCAV_AOPORTAUDIO_H
