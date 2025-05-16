//
// Created by kaelin on 5/13/25.
//

#include "ringBuffer.h"

#include <cstring>
#include <portaudio.h>

ringBuffer::ringBuffer(const int bufferSize) {
    this->bufferSize = bufferSize;
    this->buffer = new float[bufferSize];
    //memset(this->buffer, 0, bufferSize*sizeof(float));
    this->head = 0;
    this->tail = 0;
}


ringBuffer::~ringBuffer() {
    delete[] buffer;
}

void ringBuffer::writeBuffer(float *inBuffer, const int numSamples) {
    for (int i=0; i<numSamples; i++) {
        if (inBuffer[i]>0) {
            printf("value to write: %f\n", inBuffer[i]);
        }
        buffer[this->head] = inBuffer[i];
        incrimentindex( this->head);
    }
}
void ringBuffer::readBuffer(float *outBuffer, const int numSamples) {
    this->setTail(); //set tail to oldest sample
    for (int i=0; i<numSamples; i++) {
        outBuffer[i] = buffer[this->tail];
        incrimentindex(this->tail);
    }
}



void ringBuffer::incrimentindex(int &index) {
    index++;
    index &= bufferSize-1; //wrap around
}


void ringBuffer::setTail() {
    this->tail = (this->head + 1) % this->bufferSize;
}

