//
// Created by kaelin on 5/13/25.
//

#include "AudioStream.h"

//#include <bits/ranges_util.h>

AudioStream::AudioStream() { //define constructor for AudioStream class
    //initialise Port Audio Instance
    err = Pa_Initialize();
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return;
    }
    //assign input and output devices
    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {printf("no input device\n"); return;}
    inputParameters.sampleFormat = paFloat32;
    inputInfo = Pa_GetDeviceInfo(inputParameters.device);
    printf("device name: %s\n", inputInfo->name);
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {printf("no output device\n"); return;}
    outputParameters.sampleFormat = paFloat32;
    outputInfo = Pa_GetDeviceInfo(outputParameters.device);
    printf("device name: %s\n", outputInfo->name);

    // //determine lowest number of input channels (set common between input and output)
    numChannels = inputInfo->maxInputChannels < outputInfo->maxOutputChannels ? inputInfo->maxInputChannels : outputInfo->maxOutputChannels;
    //numChannels=1; //fix to mono for now
    inputParameters.channelCount = numChannels;
    outputParameters.channelCount = numChannels;

    //start a stream
    err = Pa_OpenStream( //start a stream
            &stream,
            &inputParameters,
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paNoFlag, // we won't output out of range samples so don't bother clipping them
            NULL, // no callback, use blocking I/O
            NULL); // no callback, so no callback user data
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return;
    }
    err = Pa_StartStream(stream); //start the stream





}
AudioStream::~AudioStream() { //define destructor for parent AudioStream class
    Pa_StopStream(stream);
    err = Pa_Terminate();
    if( err != paNoError )
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

}


ringBuffer *AudioStream::readAudio() {
    /*
     * This function reads audio from the input device and writes it to a new ring buffer.
     * The ring buffer is then used to process the audio data.
     */
    ringBuffer* export_buffer = new ringBuffer(FRAMES_PER_BUFFER*numChannels); //create a new ring buffer of size FRAMES_PER_BUFFER
    err = Pa_ReadStream(stream, export_buffer->buffer, FRAMES_PER_BUFFER); //read the stream
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
        return NULL;
    }
    return export_buffer; //return the ring buffer
}

void AudioStream::writeAudio(float *buffer) {
    /*
     * This function reads audio from the ring buffer and writes it to the output device.
     * The ring buffer is then used to process the audio data.
     */
    Pa_WriteStream(stream, buffer, FRAMES_PER_BUFFER); //write the stream
}

int AudioStream::getNumChannels() const {
    /*
     * This function returns the number of channels in the input device.
     */
    return numChannels;
}