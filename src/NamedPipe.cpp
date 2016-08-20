#include "File.h"
#include "NamedPipe.h"

#include <iostream>

using std::cerr;
using std::endl;
using std::cout;

namespace mlib {

NamedPipe::NamedPipe(string f, bool createNew) : 
  File(f), pipeName(f), opened(false) {
#ifdef _WIN32
  h = INVALID_HANDLE_VALUE;
  string oName = "\\\\.\\pipe\\" + pipeName;
  if (createNew) {
    initialized = false;
    h = CreateNamedPipe (oName.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES, 1024, 1024, NMPWAIT_USE_DEFAULT_WAIT, 0);
  } else {
    h
        = CreateFile (oName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    initialized = true;
  }
  if (h != INVALID_HANDLE_VALUE)
    opened = true;
#else
  if (createNew) {
    mkfifo (pipeName.c_str(), 0666);
  }
  fd = open (f.c_str(), O_RDWR);
  if (fd >= 0)
  opened = true;
#endif
}

#ifdef _WIN32
void NamedPipe::InitializePipe() {
  if (!ConnectNamedPipe(h, 0)) {
    cerr << "Impossivel conectar ao NamedPipe " << pipeName << endl;
  } else {
    cout << "Conectado ao named pipe" << endl;
    initialized = true;
  }
}
#endif

bool NamedPipe::PipeExists(string pipeName) {
#ifdef _WIN32
  string oName = "\\\\.\\pipe\\" + pipeName;
  return GetFileAttributes (oName.c_str()) != 0xFFFFFFFF;
#else
  struct stat statbuf;
  return stat (pipeName.c_str(), &statbuf) != -1;
#endif
}

unsigned long NamedPipe::Read(char* buffer, int size) {
#ifdef _WIN32
  if (!initialized) InitializePipe();
#endif
  if (!opened)
    return 0;
  unsigned long readed = 0;
#ifdef _WIN32
  ReadFile(h, buffer, size, &readed, 0);
#else
  readed = read (fd, (void*) buffer, size);
#endif
  return (int) readed;
}

string NamedPipe::GetPipeName() const {
#ifdef _WIN32
  return "\\\\.\\pipe\\" + pipeName;
#else
  return pipeName;
#endif
}

unsigned long NamedPipe::Write(char* buffer, int size) {
#ifdef _WIN32
  if (!initialized) InitializePipe();
#endif
  if (!opened)
    return 0;
  unsigned long writed = 0;
#ifdef _WIN32
  WriteFile(h, buffer, size, &writed, 0);
#else
  writed = write (fd, (void*) buffer, size);
#endif
  return writed;
}

void NamedPipe::Close() {
  if (!opened)
    return;
#ifdef _WIN32
  CloseHandle(h);
#else
  close (fd);
#endif
  opened = false;
}

NamedPipe::~NamedPipe() {
  Close();
}

}