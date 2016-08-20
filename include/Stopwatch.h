/* 
 * File:   Stopwatch.h
 * Author: marcovas
 *
 * Created on 14 de Agosto de 2016, 11:12
 */

#ifndef STOPWATCH_H
#define	STOPWATCH_H

#include "Timer.h"

namespace mlib {

    
class Stopwatch {
public:
    Stopwatch();
    void Start();
    void Stop();
    ElapsedTime GetElapsedTime();
    bool IsRunning() {
        return running;
    }
    virtual ~Stopwatch();
private:
    void CheckTime();
    unsigned long startTime;
    unsigned long currentTime;
    bool running;
#ifndef _WIN32
    timeval time0;
#endif
};

}

#endif	/* STOPWATCH_H */

