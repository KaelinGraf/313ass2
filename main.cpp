#include <iostream>
#include "portaudio.h"
#include "smbPitchShift.h"
#include "AudioStream.h"
#include "tsQueue.h"
#include <atomic>
#include <thread>
#include <condition_variable>
#include <ctime>

using namespace std;

#define PITCHFACTOR 2 // the shifting factor
#define SAMPLE_RATE 44100 // audio sampling rate
#define PA_SAMPLE_TYPE paFloat32 //32bit floating point sampling
#define FRAMES_PER_BUFFER 512 // number of frames per buffer

void pitchShiftHandler(tsQueue<ringBuffer*>& InQueue, tsQueue<ringBuffer*>& OutQueue, const atomic_flag &running, AudioStream& audioStream);

int main() {
    /* architecture for multithreading he program:
     * 1. create a thread for reading in audio from the input device. This thread continuously reads data, and places it
     *  in a new buffer, which is pushed into a fifo queue (input queue).
     * 2. create a thread for processing the audio. This thread continuously pops data from the input queue, and processes it.
     *  processed audio is placed into a new buffer, which is pushed into a fifo queue (output queue).
     * 3. Create a thread for writing audio to the output device. This thread continuously pops data from the output queue,
     * and writes it to the output device.
     *  At each stage, mutex is used to control pushing/popping from respective queues, and queue architecture is
     *  designed such that threads are held till data is available in the queue.
     *  Ideally, this will be superior to passing each buffer through induvidually, as the queue implimentation
     *  allows for "buffering" or batching of buffers, which is more efficient and should help to prevent underruns.
     *  It also prevents overruns, as the processing only takes data once its previous buffer has been processed.
     *
     *  4. event flags may be used to signal when data is available in the queue, and when the queue is empty.
     *      however, it is not clear if this is necessary, as the queue implimentation should be sufficient.
     *      Typically, flag-based implimentations would be used when queues are not viable.
     *
     *  Implimentation requirements:
     *  1. a queue class that pushes and pops in a thread safe manner.
     *  2. threads that continously call read/write and push/pop functions.
     */

    atomic_flag running = ATOMIC_FLAG_INIT; //set an atomic flag to indicate when the program is running
    running.test_and_set(); //set the flag to indicate that the program is running
    AudioStream audioStream; //create an instance of the AudioStream class
    tsQueue<ringBuffer*> inputQueue; //create a queue for the input buffer. set type as ringBuffer object
    tsQueue<ringBuffer*> outputQueue; //create a queue for the output buffer

    //call all 3 threads
    thread readThread(&AudioStream::readHandler, &audioStream, ref(inputQueue), ref(running)); //create a thread for reading audio
    //thread writeThread(&AudioStream::writeHandler, &audioStream, ref(outputQueue), ref(running)); //create a thread for writing audio
    thread pitchShiftThread(&pitchShiftHandler, ref(inputQueue), ref(outputQueue), ref(running), ref(audioStream)); //create a thread for pitch shifting audio

    //detatch the threads. they no longer need to be joined, as they will run independently of the main thread.
    readThread.detach(); //detach the read thread
    //writeThread.detach(); //detach the write thread
    pitchShiftThread.detach(); //detach the pitch shift thread

    //run for 10 seconds
    sleep(10);
    running.clear(); //clear the flag to indicate that the program is no longer running





    // while (i<10000) {
    //     ringBuffer* buffer = audioStream.readAudio(); //read audio from the input device and write it to a new ring buffer
    //     if (buffer == NULL) {
    //         std::cout << "Error reading audio" << std::endl;
    //         return -1;
    //     }
    //     //pass buffer into pitch shifter
    //     smbPitchShift(PITCHFACTOR, (FRAMES_PER_BUFFER*audioStream.getNumChannels()), 1024, 2, SAMPLE_RATE, buffer->buffer, buffer->buffer); //pitch shift the audio
    //     audioStream.writeAudio(buffer->buffer); //write the audio to the output device
    //     delete buffer;
    //     i++;
    // }
    // return 0;
}

void pitchShiftHandler(tsQueue<ringBuffer*>& InQueue, tsQueue<ringBuffer*>& OutQueue, const atomic_flag &running, AudioStream& audioStream) {
    /*
     * This function is the handler used for pitch shifting.
     * It is called in a separate thread.
     * The function continuously pops audio from the input queue, and processes it.
     * Processed audio is placed into a new buffer, which is pushed into a fifo queue (output queue).
     * When the flag is set, the function stops writing audio and exits the thread.
     *
     * i only defined it here because i cba having to redo defines
     */
    while (running.test()) {
        ringBuffer* buffer = InQueue.dequeue(); //pop audio from the input queue
        smbPitchShift(PITCHFACTOR, (FRAMES_PER_BUFFER*audioStream.getNumChannels()), 1024, 2, SAMPLE_RATE, buffer->buffer, buffer->buffer); //pitch shift the audio
        OutQueue.queue_push(buffer); //push the buffer into the output queue
    }
}

