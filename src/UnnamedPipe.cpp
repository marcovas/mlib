#include "UnnamedPipe.h"

namespace mlib {
UnnamedPipe::UnnamedPipe (): opened (false) {
#ifdef _WIN32
	if (!CreatePipe (&rPipe, &wPipe, 0, 4096))
		return;
#else
	if (pipe (fds) != 0) {
		return;
	}
#endif
   opened = true;
}

unsigned long UnnamedPipe::Read (char* buffer, int size) {
   if (!opened)
      return 0;
	unsigned long readed = 0;
#ifdef _WIN32
	ReadFile (rPipe, buffer, size, &readed, 0);
#else
   readed = read (fds [0], buffer, size);
#endif
   if (readed <= 0 && size > 0) {
      Close ();
      return 0;
   }
	return (unsigned long) readed;
}

unsigned long UnnamedPipe::Write (char* buffer, int size) {
   if (!opened)
      return 0;
	unsigned long writed = 0;
#ifdef _WIN32
	WriteFile (wPipe, buffer, size, &writed, 0);
#else
   writed = write (fds [1], buffer, size);
#endif
   if (writed <= 0 && size > 0) {
      Close ();
      return 0;
   }
	return (unsigned long) writed;
}

void UnnamedPipe::Close() {
   if (!opened)
      return;
#ifdef _WIN32
	CloseHandle (wPipe);
	CloseHandle (rPipe);
#else
   close (fds [0]);
   close (fds [1]);
#endif
   opened = false;
}

UnnamedPipe::~UnnamedPipe () {
	Close();
}
}