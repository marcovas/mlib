/* 
 * File:   AutoThread.h
 * Author: marcovas
 *
 * Created on 15 de Agosto de 2016, 05:44
 */

#ifndef AUTOTHREAD_H
#define	AUTOTHREAD_H

#include "PThread.h"

/*
 * Classe que supervisiona uma thread e logo após, invoca um delete
 * sobre a thread e sobre si própria
 * Use somente como ponteiro AutoThread t = new AutoThread(Thread *t)
 * pois esta invocará delete this ao final da execução
 */

namespace mlib {

    class AutoThread: public PThread  {
    public:
        AutoThread(PThread *t);
        virtual void Main();
        virtual ~AutoThread();
    private:
        PThread *thread;
    };

}
#endif	/* AUTOTHREAD_H */

