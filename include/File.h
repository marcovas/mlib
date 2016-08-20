#ifndef _STREAM_H
#define _STREAM_H

#include "Stream.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#endif

#include <string>

using std::string;

namespace mlib {
class File: public Stream {
public:

   /*static const unsigned short READ_ONLY = 0x1;
   static const unsigned short WRITE_ONLY = 0x2;
   static const unsigned short READ_WRITE = 0x4;*/

  enum open_mode { READ_ONLY = 0x1, WRITE_ONLY, READ_WRITE};
	File (string fileName, bool createNew = false, const open_mode mode = READ_WRITE);
	virtual void Close ();
	virtual unsigned long Read (char*, int);
	virtual unsigned long Write (char*, int);
	virtual bool Delete();
	static bool Mkdir(const string &);
	static bool Rmdir(const string&);
	bool IsDirectory() const;
	bool FileExists () const;
	unsigned long GetSize() const;
	bool Create ();
	string GetName () const { return fileName; }
	virtual bool IsOpened () const { return opened; }
	virtual bool Seek(unsigned long long);
	virtual ~File ();
protected:
#ifdef _WIN32
	HANDLE h;
#else
	int fd;
#endif
	string fileName;
	bool opened;
	enum open_mode openMode;
};

}

#endif
