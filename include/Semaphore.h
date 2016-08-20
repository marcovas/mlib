#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <semaphore.h>
#endif
namespace mlib {

class Semaphore
{
public:
	Semaphore(unsigned int);
	bool Wait ();
	bool Signal ();
	unsigned int getInitialCount () { return initialCount; }
	~Semaphore();
protected:
	unsigned int initialCount;
#ifdef _WIN32
	HANDLE handle;
#else
	sem_t semID;
#endif
};

class WaitAndSignal {
	public:
	WaitAndSignal (Semaphore& s): sem(s){
		sem.Wait();
	}
	~WaitAndSignal () {
		sem.Signal ();
	}
	private:
		Semaphore sem;
};

}

#endif //_SEMAPHORE_H_
