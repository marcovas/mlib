#ifndef _NAMEDPIPE_H
#define _NAMEDPIPE_H

#include <File.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <string>

using std::string;

namespace mlib {

class NamedPipe: public File {
public:
	NamedPipe (string pipeName, bool createNew = false);
	virtual void Close ();
	virtual unsigned long Read (char*, int);
	virtual unsigned long Write (char*, int);
	static bool PipeExists(string pipeName);
	bool Create ();
	string GetName () const { return pipeName; }
	string GetPipeName() const;
	virtual bool IsOpened () const { return opened; }
	virtual ~NamedPipe ();
protected:
#ifdef _WIN32
  void InitializePipe();
	HANDLE h;
	bool initialized;
#else
	int fd;
#endif
	string pipeName;
	bool opened;
};

}

#endif
