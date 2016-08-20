/* 
 * File:   AutoThread.cpp
 * Author: marcovas
 * 
 * Created on 15 de Agosto de 2016, 05:44
 */

#include "AutoThread.h"

namespace mlib {

    AutoThread::AutoThread(PThread *t): thread(t){
    }
    
    void AutoThread::Main() {
        thread->Start();
        thread->Wait();       
        delete this;
    }
    
    AutoThread::~AutoThread() {
        delete thread;
    }

}
