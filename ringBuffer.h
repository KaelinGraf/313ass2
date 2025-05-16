//
// Created by kaelin on 5/13/25.
//

#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <array>
#include <stdio.h>


class ringBuffer {
/*
 * This class implements a ring buffer of size bufferSize. We fill up the buffer on each sampling instance
 *
 */
private:
    int bufferSize;
    int head;
    int tail;
    //float* buffer;
public:
    float* buffer;
    ringBuffer(int bufferSize);//create a buffer of capacity bufferSize.
    ~ringBuffer(); //Delete buffer
    void writeBuffer(float *inBuffer, int numSamples); //write to the buffer
    void readBuffer(float* outBuffer, int numSamples); //read from the buffer
    void incrimentindex(int& index); //increment the index (head or tail), and wrap around if necessary
    void setTail();// sets tail to oldest sample (checks if buffer has been filled)
};



#endif //RINGBUFFER_H
