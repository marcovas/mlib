#include "Mutex.h"

namespace mlib {

Mutex::Mutex() : Semaphore (1)
{
}

}