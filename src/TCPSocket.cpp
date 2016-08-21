#include "TCPSocket.h"

#include <iostream>
#include <cstdio>
using std::cout;
using std::cerr;
using std::endl;

namespace mlib {

#ifdef _WIN32

TCPSocket::TCPSocket(SOCKET fd) :
    MSocket(), opened(false), addr(""), sckt(fd) {
        cout << "Created socket is " << fd << endl;
    }
#else
#include <unistd.h>

TCPSocket::TCPSocket(int fd) :
    MSocket(), opened(false), addr(""), sckt(fd) {
        cout << "Created socket is " << fd << endl;
    }
#endif

    TCPSocket::TCPSocket(unsigned short port) :
    MSocket(port) {
    }

    TCPSocket::TCPSocket(string a, unsigned short port) :
    MSocket(port), opened(false), addr(a) {
#ifdef _WIN32
        sckt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sckt == INVALID_SOCKET) {
            perror("Error creating socket");
            return;
        }
#else
        sckt = socket(PF_INET, SOCK_STREAM, 0);
        if (sckt < 0) {
            perror("Error creating socket");
            return;
        }
        cout << "Created socket is " << sckt << endl;
#endif
        cout << "Socket OK" << endl;
        opened = true;
    }

#ifdef _WIN32

TCPSocket::TCPSocket() :
    MSocket(0), sckt(0) {
        sckt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sckt == INVALID_SOCKET) {
            perror("Error creating socket");
            return;
        }
    }
#else

TCPSocket::TCPSocket() :
    MSocket(0), sckt(0) {
        sckt = socket(PF_INET, SOCK_STREAM, 0);
        if (sckt < 0) {
            perror("Error creating socket");
            return;
        }
        cout << "Created socket is " << sckt << endl;
    }
#endif

    TCPSocket::~TCPSocket() {
        Close();
        dealocate();
    }

    bool TCPSocket::Bind() {
        cout << "Binding " << addr << endl;
#ifdef _WIN32
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr(addr.c_str());
        service.sin_port = htons(port);
        bool ok = false;
        if (bind(sckt, (SOCKADDR*) & service, sizeof (service)) != SOCKET_ERROR)
            ok = true;
        if (!ok) {
            cerr << "Error binding socket" << endl;
            return false;
        }
#else
        service.sin_family = AF_INET;
        service.sin_port = htons(port);
        service.sin_addr.s_addr = inet_addr(addr.c_str());
        memset(service.sin_zero, '\0', sizeof (service.sin_zero));
        int result = bind(sckt, (struct sockaddr *) &service, sizeof (service));
        if (result < 0) {
            perror("Cannot bind port TCP ");
            return false;
        }
#endif
        cout << "Bind sucessfully" << endl;
        opened = true;
        return true;
    }

    int TCPSocket::Read(unsigned char* buffer, unsigned short len) {
        WaitAndSignal r(readMutex);
        if (!opened || len == 0) {
            cerr << "!opened || len == 0 falhou" << endl;
            return 0;
        }
        int read = 0;
#ifdef _WIN32
        read = recv(sckt, (char*) buffer, len, 0);
        if (read <= 0 || read == WSAECONNRESET) {
            Close();
            cerr << "Unexpected socket close" << endl;
            read = 0;
        }
#else
        cout << "Reading from " << sckt << endl;
        read = recv(sckt, (void*) buffer, len, 0);
        if (read < 0) {
            perror("Unexpected socket close");
            read = 0;
        }
#endif
        return read;
    }

    void TCPSocket::Close() {
        WaitAndSignal r(readMutex);
        WaitAndSignal w(writeMutex);
        cout << "TCPSocket::Close " << sckt << endl;
        if (opened) {
#ifdef _WIN32
            closesocket(sckt);
#else
            if (close(sckt) == -1) {
                perror("Error in close");
            } else {
                cerr << "Close OK" << endl;
            }
#endif
            opened = false;
        }
    }

    bool TCPSocket::Write(unsigned char *buffer, unsigned short len) {
        WaitAndSignal w(writeMutex);
        if (!opened || len == 0) {
            cerr << "!opened || len == 0" << endl;
            return false;
        }
        int sent = 0;
#ifdef _WIN32
        sent = send(sckt, (char*) buffer, len, 0);
#else
        cout << "Writing to " << sckt << endl;
        sent = send(sckt, (char*) buffer, len, 0);
#endif
        if (sent > 0)
            return true;
        perror("Error in Write");
        return false;
    }

    TCPSocket * TCPSocket::Accept() {
#ifdef _WIN32
        struct sockaddr_in raddr;
        int rlen = sizeof (raddr);
        cerr << "Accepting socket..." << endl;
        SOCKET fd = accept(sckt, (sockaddr*) & raddr, &rlen);
        if (fd != INVALID_SOCKET) {
            cerr << "Accepted socket is " << fd << endl;
            TCPSocket *socket = new TCPSocket(fd);
            remoteAddress = string(inet_ntoa(raddr.sin_addr));
            remotePort = raddr.sin_port;
            socket->addr = remoteAddress;
            socket->port = remotePort;
            socket->opened = true;
            return socket;
        } else
            cerr << "Erro no accept: " << WSAGetLastError() << endl;
#else
        struct sockaddr_in raddr;
        socklen_t addrlen = sizeof (raddr);
        int fd = accept(sckt, (struct sockaddr*) &raddr, &addrlen);
        if (fd >= 0) {
            cout << "Accepted socket is " << fd << endl;
            TCPSocket *socket = new TCPSocket(fd);
            remoteAddress = string(inet_ntoa(raddr.sin_addr));
            remotePort = raddr.sin_port;
            socket->addr = remoteAddress;
            socket->port = remotePort;
            socket->opened = true;
            return socket;
        }
#endif
        perror("Erro no Accept");
        return 0;
    }

    bool TCPSocket::Listen(int backlog) {
#ifdef _WIN32
        if (listen(sckt, backlog) != 0) {
            perror("Error in listen");
            return false;
        }
#else
        if (listen(sckt, backlog) < 0) {
            perror("Error in listen");
            return false;
        }
#endif
        return true;
    }

    bool TCPSocket::Connect(string raddr, unsigned int port) {
        remote.sin_family = AF_INET;
        remote.sin_port = htons(port);
        remote.sin_addr.s_addr = inet_addr(raddr.c_str());
        memset(remote.sin_zero, '\0', sizeof (remote.sin_zero));
#ifdef _WIN32
        if (connect(sckt, (SOCKADDR*) & remote, sizeof (remote)) == SOCKET_ERROR) {
            perror("Error opening socket");
            return false;
        }
#else
        if (connect(sckt, (struct sockaddr*) &remote, sizeof (remote)) < 0) {
            perror("Error in connect");
            return false;
        }
#endif
        remoteAddress = raddr;
        remotePort = port;
        opened = true;
        return true;
    }

}
