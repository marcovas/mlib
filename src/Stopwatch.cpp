/* 
 * File:   Stopwatch.cpp
 * Author: marcovas
 * 
 * Created on 14 de Agosto de 2016, 11:12
 */

#include "Stopwatch.h"

namespace mlib {

Stopwatch::Stopwatch(): startTime(0), currentTime(0), running(false) {
}

void Stopwatch::Start() {
    if (!running) {
#ifndef _WIN32
    gettimeofday (&time0, 0);
    startTime = timeval2int (&time0);
#else
    startTime = GetTickCount();
#endif   //_WIN32
    }
    running = true;
}

void Stopwatch::Stop() {
    if (running) {
        CheckTime();
    }
    running = false;
}

void Stopwatch::CheckTime() {
    if (running) {
#ifndef _WIN32
    timeval current;
    gettimeofday (&current, 0);
    currentTime = timeval2int (&current);
#else
    currentTime = GetTickCount();
#endif   //_WIN32
    }
}

ElapsedTime Stopwatch::GetElapsedTime() {
    CheckTime();
    return ElapsedTime(startTime, currentTime);
}

Stopwatch::~Stopwatch() {
    Stop();
}

}
