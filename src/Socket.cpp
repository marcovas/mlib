#include "Socket.h"

#include <iostream>

using std::cout;
using std::endl;

#ifdef _WIN32

BOOL APIENTRY DllMain(HANDLE hModule, // Handle to DLL module
    DWORD ul_reason_for_call, LPVOID lpReserved) // Reserved
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    cout << "@@@@@@@@@@@ Biblioteca carregada" << endl;
    break;
  case DLL_THREAD_ATTACH:
    cout << "@@@@@@@@@@@ A process is creating a new thread." << endl;
    break;
  case DLL_THREAD_DETACH:
    cout << "@@@@@@@@@@ A thread exits normally." << endl;
    break;
  case DLL_PROCESS_DETACH:
    cout << "@@@@@@@@@@@ Bibliotea descarregada" << endl;
    break;
  }
  return TRUE;
}

static bool initialised = false;

#else
#endif

namespace mlib {

static int refCount = 0;

static void initialise() {
#ifdef _WIN32
	if (initialised) {
		return;
	}
  WSADATA wsaData;
  if (refCount++ == 0) {
    cout << "Initialising win sockets" << endl;
    int iResult = WSAStartup(MAKEWORD (2, 0), &wsaData);
    if (iResult != NO_ERROR)
      cout << "Error initialising WinSock" << endl;
    else {
      initialised = true;
      cout << "WinSock sucessfully initialised" << endl;
    }
  }
#else
#endif
}

MSocket::MSocket() {
  //if (!initialised)
  initialise();
  port = 0;
}

MSocket::MSocket(unsigned short p) {
  //if (!initialised)
  initialise();
  port = p;
}

void MSocket::dealocate() {
  if (refCount == 0) {
#ifdef _WIN32
    cout << "Cleaning up WSA services (refCount == " << refCount << ')' << endl;
    return;
    //WSACleanup();
#else

#endif
  }
  refCount--;
}

void MSocket::SetPort(unsigned short p) {
  port = p;
}

unsigned short MSocket::GetPort() const {
  return port;
}

bool MSocket::SetRecvBufferSize(unsigned size) {
#ifdef _WIN32
  char * opt = (char *)&size;
#else
  const void* opt = (const void*) &size;
#endif
  int res = setsockopt(sckt, SOL_SOCKET, SO_RCVBUF, opt, sizeof(size));
  if (res == 0)
    return true;
  return false;
}

bool MSocket::SetSendBufferSize(unsigned size) {
#ifdef _WIN32
  char * opt = (char *)&size;
#else
  const void* opt = (const void*) &size;
#endif
  int res = setsockopt(sckt, SOL_SOCKET, SO_SNDBUF, opt, sizeof(size));
  if (res == 0)
    return true;
  return false;
}

unsigned MSocket::GetRecvBufferSize() const {
  int size = 0;
#ifdef _WIN32
  char * opt = (char *)&size;
  int optlen = sizeof(*opt);
#else
  void * opt = (void*) &size;
  socklen_t optlen = sizeof(size);
#endif
  int res = getsockopt(sckt, SOL_SOCKET, SO_RCVBUF, opt, &optlen);
  if (res == 0)
    return size;
  return 0;
}

unsigned MSocket::GetSendBufferSize() const {
  int size = 0;
#ifdef _WIN32
  char * opt = (char *)&size;
  int optlen = sizeof(*opt);
#else
  void * opt = (void*) &size;
  socklen_t optlen = sizeof(size);
#endif
  int res = getsockopt(sckt, SOL_SOCKET, SO_SNDBUF, opt, &optlen);
  if (res == 0)
    return size;
  return 0;
}

MSocket::~MSocket() {
  dealocate();
}

}
