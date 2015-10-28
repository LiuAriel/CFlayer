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


#include <CCAV/port_audio.h>
#include <private/port_audio_private.h>

namespace CCAV {

CCPortAudio::CCPortAudio()
    :CCOutputAudio(*new CCPortAudioPrivate())
{
}

CCPortAudio::~CCPortAudio()
{
    close();
}

bool CCPortAudio::write()
{
	DPTR_D(CCPortAudio);
	if (Pa_IsStreamStopped(d.stream))
		Pa_StartStream(d.stream);

	PaError err = Pa_WriteStream(d.stream, d.data.data(), d.data.size() / d.channels / sizeof(float));
	if (err == paUnanticipatedHostError) {
		qWarning("Write portaudio stream error: %s", Pa_GetErrorText(err));
		return false;
	}
	return true;
}


bool CCPortAudio::open()
{
    DPTR_D(CCPortAudio);
    //
    d.outputParameters->channelCount = d.channels;
    PaError err = Pa_OpenStream(&d.stream, NULL, d.outputParameters, d.sample_rate, 0, paNoFlag, NULL, NULL);
    if (err == paNoError) {
        d.outputLatency = Pa_GetStreamInfo(d.stream)->outputLatency;
        d.available = true;
    } else {
        qWarning("Open portaudio stream error: %s", Pa_GetErrorText(err));
        d.available = false;
    }
    return err == paNoError;
}

bool CCPortAudio::close()
{
    DPTR_D(CCPortAudio);
    PaError err = paNoError;
    if (!d.stream) {
        return true;
    }
    err = Pa_StopStream(d.stream);
    if (err != paNoError)
        qWarning("Stop portaudio stream error: %s", Pa_GetErrorText(err));
    err = Pa_CloseStream(d.stream);
    d.stream = NULL;
    if (err != paNoError)
        qWarning("Close portaudio stream error: %s", Pa_GetErrorText(err));
    return err == paNoError;
}

} //namespace CCAV
