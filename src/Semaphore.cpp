#include "Semaphore.h"

namespace mlib {

Semaphore::Semaphore(unsigned int i): initialCount (i), closed(false)
{
#ifndef _WIN32
   sem_init (&semID, 1, initialCount);
#else
   handle = CreateSemaphore (0, initialCount, 300, 0);
#endif
}

bool Semaphore::Wait() {
	if (closed)
		return false;
#ifndef _WIN32
   sem_wait (&semID);
#else
   WaitForSingleObject (handle, INFINITE);
#endif
   return true;
}

bool Semaphore::Signal() {
	if (closed)
		return false;
#ifndef _WIN32
   sem_post (&semID);
#else
   ReleaseSemaphore (handle, 1, 0);
#endif
   return true;
}

void Semaphore::Close() {
	if (closed)
		return;
#ifndef _WIN32
   sem_destroy (&semID);
#else
   CloseHandle (handle);
#endif
   closed = true;
}

Semaphore::~Semaphore()
{
	Close();
}

}
