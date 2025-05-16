//
// Created by kaelin on 5/13/25.
//

#include "audioReader.h"


audioReader::audioReader() {
    inputParameters.device=Pa_GetDefaultInputDevice();
}
