#ifndef _M_TCP_SOCKET_H
#define _M_TCP_SOCKET_H

#include "Socket.h"
#include <string>
#include <cstring>

using std::string;

namespace mlib {

class TCPSocket : public MSocket {
public:
   TCPSocket(string addr, unsigned short port);
   TCPSocket ();
   virtual ~TCPSocket();
   bool Bind();
   bool Listen(int backlog);
   TCPSocket * Accept();
   string GetAddress() const {
      return addr;
   }
   string GetRemoteAddress() const { return remoteAddress; }
   bool Connect(string, unsigned int);
   int Read(unsigned char*, unsigned short);
   bool Write(unsigned char *buffer, unsigned short length);
// Stream
   unsigned long Read (char* b, int i) { return Read((unsigned char*)b, i); }
   unsigned long Write (char* b, int i) { return Write((unsigned char*)b, i); }
   bool IsOpened () const {
      return opened;
   }
// Stream - end
   void Close();
protected:
   TCPSocket(unsigned short port);
#ifdef _WIN32
   TCPSocket(SOCKET);
#else
   TCPSocket(int);
#endif
   bool opened;
   string addr;
#ifdef _WIN32
   SOCKET sckt;
   sockaddr_in client;
#else
   int sckt;
#endif
   sockaddr_in service;
   sockaddr_in remote;
   string remoteAddress;
   unsigned short remotePort;
};

}

#endif
