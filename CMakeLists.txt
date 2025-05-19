cmake_minimum_required(VERSION 3.31)
project(313ass2)

set(CMAKE_CXX_STANDARD 26)

find_package(portaudio REQUIRED HINTS /usr/local/lib/)

add_executable(313ass2 main.cpp
        smbPitchShift.cpp
        smbPitchShift.h
        AudioStream.cpp
        AudioStream.h
        ringBuffer.cpp
        ringBuffer.h
        tsQueue.h)



target_link_libraries(313ass2 PRIVATE portaudio)

