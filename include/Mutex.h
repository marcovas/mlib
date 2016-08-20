#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "Semaphore.h"

namespace mlib {

class Mutex : public Semaphore
{
public:
	Mutex();
};

}

#endif //_MUTEX_H_
