#ifndef SYNCPOINT_H_
#define SYNCPOINT_H_

#include "Semaphore.h"

namespace mlib {

class SyncPoint: public Semaphore {
public:
   SyncPoint() : Semaphore(0){}
};

}

#endif /*SYNCPOINT_H_*/
