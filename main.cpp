#include <iostream>
#include "portaudio.h"
#include "smbPitchShift.h"
#include "AudioStream.h"

#define PITCHFACTOR 2 // the shifting factor
#define SAMPLE_RATE 44100 // audio sampling rate
#define PA_SAMPLE_TYPE paFloat32 //32bit floating point sampling
#define FRAMES_PER_BUFFER 512 // number of frames per buffer
int main() {
    AudioStream audioStream; //create an instance of the AudioStream class
    int i=0;
    while (i<10000) {
        ringBuffer* buffer = audioStream.readAudio(); //read audio from the input device and write it to a new ring buffer
        if (buffer == NULL) {
            std::cout << "Error reading audio" << std::endl;
            return -1;
        }
        //pass buffer into pitch shifter
        smbPitchShift(PITCHFACTOR, (FRAMES_PER_BUFFER*audioStream.getNumChannels()), 1024, 2, SAMPLE_RATE, buffer->buffer, buffer->buffer); //pitch shift the audio
        audioStream.writeAudio(buffer->buffer); //write the audio to the output device
        delete buffer;
        i++;
    }
    return 0;
}



