//
// Created by kaelin on 5/13/25.
//

#ifndef AUDIOREADER_H
#define AUDIOREADER_H
#include "AudioStream.h"


class audioReader: public AudioStream{
    private:
    PaStreamParameters inputParameters{};
    public:
    audioReader();
};



#endif //AUDIOREADER_H
