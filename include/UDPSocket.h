#ifndef _M_UDP_SOCKET_H
#define _M_UDP_SOCKET_H

#include "Socket.h"
#include <string>
#include <cstring>
using std::string;

namespace mlib {

    class UDPSocket : public MSocket {
    public:
        UDPSocket(unsigned short port);
        bool Bind(const string&);
        virtual ~UDPSocket();
        int Read(unsigned char*, unsigned short);
        string Read();
        bool Write(unsigned char *, unsigned short);
        bool Write(const string &);
        virtual void Close();
        void SetAddress(const string&);

        string GetAddress() const {
            return addr;
        }
        string GetRemoteAddress() const;

        unsigned short GetRemotePort() const {
            return remotePort;
        }

        bool IsBroadcast() const;
        void SetBroadcast();

    protected:
        string addr;
        bool success;
        sockaddr_in service;
        sockaddr_in remote;
        string remoteAddress;
        unsigned short remotePort;
        bool broadcast;
    };
    bool operator==(const UDPSocket &, const UDPSocket &);
    
}

#endif
