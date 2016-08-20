#ifndef _NATIVE_SOCKET_H
#define _NATIVE_SOCKET_H

#include <Mutex.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace mlib {

class MSocket {
public:
   MSocket (unsigned short port);
   virtual ~MSocket();
   void SetPort (unsigned short port);
   unsigned short GetPort () const;
   virtual int Read (unsigned char *buffer, unsigned short length) = 0;
   virtual bool Write (unsigned char *buffer, unsigned short length) = 0;
   virtual void Close () = 0;
   virtual bool IsClosed() { return closed; }
   virtual bool SetRecvBufferSize(unsigned);
   virtual bool SetSendBufferSize(unsigned);
   virtual unsigned GetRecvBufferSize() const;
   virtual unsigned GetSendBufferSize() const;
protected:
   MSocket();
#ifdef _WIN32
   SOCKET sckt;
#else
   int sckt;
#endif
   virtual void dealocate ();
   unsigned short port;
   bool closed;
   Mutex writeMutex;
   Mutex readMutex;
};

}

#endif
