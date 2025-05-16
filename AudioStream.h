//
// Created by kaelin on 5/13/25.
//


#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H
#define SAMPLE_RATE 44100 // audio sampling rate
#define PA_SAMPLE_TYPE paFloat32 //32bit floating point sampling
#define FRAMES_PER_BUFFER 512
#define FRAMES_PER_BLOCK 512

#include "portaudio.h"
#include <stdio.h>
#include "ringBuffer.h"

class AudioStream {
    private:
    PaError err=paNoError;
    PaStream *stream=NULL; //using blocking I/O
    PaStreamParameters inputParameters{}, outputParameters{};
    const PaDeviceInfo *inputInfo,*outputInfo;
    int numChannels;

    public:
    AudioStream();
    ~AudioStream();
    ringBuffer* readAudio();
    void writeAudio(float* buffer);
    int getNumChannels() const;
};



#endif //AUDIOSTREAM_H
