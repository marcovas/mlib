#ifndef _PTHREAD_H_
#define _PTHREAD_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#endif

#include "Mutex.h"

namespace mlib {

    class PThread {
    public:
        PThread();
        PThread(void*);
        virtual ~PThread();
        virtual void Main() = 0;
        bool IsRunning();
        static void Sleep(long);
        virtual void Start();
        void Suspend();
        void Resume();
        void Stop();
        void Restart();
        void NextThread();
        void Wait();

    protected:
#ifdef _WIN32
        HANDLE handle;
#else
        pthread_t threadID;
#endif
        void* args;
        bool running;
        Mutex mutex;
    };

}
#endif //_PTHREAD_H_
