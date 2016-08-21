#include "File.h"

#ifndef _WIN32
#include <cstdio>
#endif

namespace mlib {

File::File(string f, bool createNew, const open_mode mode) :
  fileName(f), opened(false), openMode(mode) {
#ifdef _WIN32
  h = INVALID_HANDLE_VALUE;
  DWORD flags = GENERIC_READ;
  switch (mode) {
  case READ_ONLY:
    flags = GENERIC_READ;
    break;
  case WRITE_ONLY:
    flags = GENERIC_WRITE;
    break;
  case READ_WRITE:
    flags = GENERIC_READ | GENERIC_WRITE;
  }
  if (createNew) {
    h = CreateFile (fileName.c_str(), flags, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  } else {
    h = CreateFile (fileName.c_str(), flags, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  }
  if (h != INVALID_HANDLE_VALUE)
    opened = true;
#else
  int flags = READ_ONLY;
  switch (mode) {
    case READ_ONLY:
    flags = O_RDONLY;
    break;
    case WRITE_ONLY:
    flags = O_WRONLY;
    break;
    case READ_WRITE:
    flags = O_RDWR;
  }
  if (createNew)
    fd = open (fileName.c_str(), flags);
  else
    fd = open (fileName.c_str(), flags);
  if (fd> 0)
  opened = true;
#endif
}

bool File::Mkdir(const string& dirName) {
#ifdef _WIN32
  if (CreateDirectory(dirName.c_str(), 0))
    return true;
#else
  if (mkdir (dirName.c_str(), 0777) == 0)
    return true;
#endif
  return false;
}

bool File::Seek(unsigned long long pos) {
#ifdef _WIN32
  LARGE_INTEGER distanceToMove;
  distanceToMove.QuadPart = pos;
  if (SetFilePointerEx(h, distanceToMove, 0, FILE_BEGIN))
    return true;
#else
  if (lseek (fd, SEEK_SET, pos) != -1)
    return true;
#endif
  return false;
}

bool File::Rmdir(const string& dirName) {
#ifdef _WIN32
  if (RemoveDirectory(dirName.c_str()))
    return true;
#else
  if (remove(dirName.c_str()) == 0)
    return true;
#endif
  return false;
}

unsigned long File::GetSize() const {
#ifdef _WIN32
   return GetFileSize(h, 0);
#else
   struct stat buff;
   stat (fileName.c_str(), &buff);
   return buff.st_size;
#endif
}

bool File::IsDirectory() const {
#ifdef _WIN32
  return (GetFileAttributes (fileName.c_str()) & FILE_ATTRIBUTE_DIRECTORY);
#else
  struct stat fileStat;
  stat (fileName.c_str(), &fileStat);
#endif
  return false;
}

bool File::Delete() {
  const char *cFileName = fileName.c_str();
  Close();
#ifdef _WIN32
  if (DeleteFile(cFileName))
    return true;
#else
  if (remove(cFileName) == 0)
    return true;
#endif
  return false;
}

bool File::FileExists() const {
#ifdef _WIN32
  return GetFileAttributes (fileName.c_str()) != 0xFFFFFFFF;
#else
  struct stat buff;
  return (stat (fileName.c_str(), &buff) != -1);
#endif
}

unsigned long File::Read(char* buffer, int size) {
  if (!opened)
    return 0;
  unsigned long readed = 0;
#ifdef _WIN32
  ReadFile(h, buffer, size, &readed, 0);
#else
  readed = read (fd, buffer, size);
#endif
  if (readed < 0)
    return 0;
  return (unsigned long) readed;
}

unsigned long File::Write(char* buffer, int size) {
  if (!opened)
    return 0;
  unsigned long writed = 0;
#ifdef _WIN32
  WriteFile(h, buffer, size, &writed, 0);
#else
  writed = write (fd, buffer, size);
#endif
  if (writed < 0)
    return 0;
  return (unsigned long) writed;
}

void File::Close() {
  if (!opened)
    return;
#ifdef _WIN32
  CloseHandle(h);
  opened = false;
#else
  close (fd);
#endif
  opened = false;
}

File::~File() {
  Close();
}

}
