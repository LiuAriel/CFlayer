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


#ifndef CCAV_AOPORTAUDIO_P_H
#define CCAV_AOPORTAUDIO_P_H

#include <private/output_audio_private.h>
#include <portaudio.h>
#include "master_array.h"
namespace CCAV {

class Q_EXPORT CCPortAudioPrivate : public CCOutputAudioPrivate
{
public:
    CCPortAudioPrivate():outputParameters(new PaStreamParameters)
      ,stream(0),initialized(true)
    {
        PaError err = paNoError;
        if ((err = Pa_Initialize()) != paNoError) {
            qWarning("Error when init portaudio: %s", Pa_GetErrorText(err));
            available = false;
            initialized = false;
            return;
        }

        memset(outputParameters, 0, sizeof(PaStreamParameters));
        outputParameters->device = Pa_GetDefaultOutputDevice();
        if (outputParameters->device == paNoDevice) {
            qWarning("PortAudio get device error!");
            available = false;
            return;
        }
        outputParameters->sampleFormat = paFloat32;
        outputParameters->hostApiSpecificStreamInfo = NULL;
        outputParameters->suggestedLatency = Pa_GetDeviceInfo(outputParameters->device)->defaultHighOutputLatency;

    }
    ~CCPortAudioPrivate() {
        if (initialized)
            Pa_Terminate(); 
        if (outputParameters) {
            delete outputParameters;
            outputParameters = 0;
        }
    }

    bool initialized;
    PaStreamParameters *outputParameters;
    PaStream *stream;

    double outputLatency;
};

} //namespace CCAV
#endif // CCAV_AOPORTAUDIO_P_H
