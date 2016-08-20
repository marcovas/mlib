#ifndef _UNNAMEDPIPE_H
#define _UNNAMEDPIPE_H

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <Stream.h>

namespace mlib {

class UnnamedPipe: public Stream {
public:
	UnnamedPipe ();
	void Close ();
	virtual unsigned long Read (char*, int);
	virtual unsigned long Write (char*, int);
   virtual bool IsOpened () const { return opened; }
	~UnnamedPipe();
private:
#ifdef _WIN32
	HANDLE wPipe; // Handle to write-side pipe
	HANDLE rPipe; // Handle to read-side pipe
#else
	int fds [2];
#endif
   bool opened;
};

}

#endif
