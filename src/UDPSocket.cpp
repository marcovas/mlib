#include "UDPSocket.h"
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <unistd.h>

#include <cstdio>

namespace mlib {

    UDPSocket::UDPSocket(unsigned short port) : MSocket(port),
    addr(""), success(false), remoteAddress(""), remotePort(0),
	broadcast(false) {
        sckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#ifdef _WIN32
        if (sckt == INVALID_SOCKET) {
            cerr << "Error creating socket" << endl;
            return;
        }
#else
        if (sckt < 0) {
            cerr << "Error creating socket" << endl;
            return;
        }
#endif
        success = true;
        cout << "Socket sucessfully created" << endl;
        closed = false;
    }

    UDPSocket::~UDPSocket() {
        Close();
        dealocate();
    }

    bool UDPSocket::Write(const string &s) {
        return Write((unsigned char*) s.c_str(), s.length());
    }

    bool UDPSocket::IsBroadcast() const {
    	return broadcast;
    }

    string UDPSocket::Read() {
    	char buff[1500];
    	int readed = Read((unsigned char*)buff, 1500);
    	if (readed > 0) {
    		return string((char*)buff, 0, readed);
    	}
    	return "";
    }

    int UDPSocket::Read(unsigned char* buffer, unsigned short len) {
        WaitAndSignal m(readMutex);
        if (broadcast) {
        	return 0;
        }
#ifdef _WIN32
        int read = recv(sckt, (char*) buffer, len, 0);
        return read;
#else
        socklen_t slen = sizeof (remote);
        struct sockaddr addr;
        int read = recvfrom(sckt, buffer, len, 0, &addr, &slen);
        if (read > 0) {
        	memcpy(&remote, &addr, slen);
        	remoteAddress = string(inet_ntoa(remote.sin_addr));
        	remotePort = remote.sin_port;
        }
        return read;
#endif
    }

    void UDPSocket::Close() {
        if (closed)
            return;
#ifdef _WIN32
        closesocket(sckt);
#else
        shutdown(sckt, 2);
        close(sckt);
#endif
        readMutex.Close();
        writeMutex.Close();
        closed = true;
    }

    bool UDPSocket::Write(unsigned char* buffer, unsigned short len) {
        WaitAndSignal w(writeMutex);
        if ((!broadcast && addr == "") || len == 0)
            return false;
        int sent = send(sckt, (char*) buffer, len, 0);
        if (sent < 0) {
#ifdef _WIN32
        	cerr << "Erro ao enviar bytes: " << WSAGetLastError() << endl;
#else
        	cerr << "Erro ao enviar bytes (UDPSocket)" << sckt<< endl;
        	perror("ERRO no UDPSocket.Write");
#endif
        }
        return sent >= 0;
    }

    string UDPSocket::GetRemoteAddress() const {
        return remoteAddress;
    }

    void UDPSocket::SetAddress(const string &s) {
    	if (broadcast) {
    		return;
    	}
        addr = s;
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr(s.c_str());
        service.sin_port = htons(port);
        int result;
#ifdef _WIN32
        result = connect(sckt, (SOCKADDR*) & service, sizeof (service));
        if (result == SOCKET_ERROR)
            cerr << "Error in UDPSocket::SetAddress" << endl;
#else
        result = connect(sckt, (struct sockaddr *) &service, sizeof (service));
        if (result < 0) {
            cerr << "Error in UDPSocket::SetAddress" << endl;
            perror("ERRO");
        }
#endif
    }

    void UDPSocket::SetBroadcast() {
    	if (addr != "") {
    		return;
    	}
    	int yes = 1;
#ifdef _WIN32
    	char * opt = (char *)&yes;
#else
    	const void* opt = (const void*) &yes;
#endif
    	int ret = setsockopt(sckt, SOL_SOCKET, SO_BROADCAST, opt, sizeof(yes));
#ifdef _WIN32
    	if (ret == SOCKET_ERROR) {
    		int err = WSAGetLastError();
    		cerr << "Erro ao setar opções: " << err << endl;
    	}
#else
    	if (ret) {
    		perror("Erro ao setar broadcast");
    		return;
    	}
#endif

    	SetAddress("255.255.255.255");
    	broadcast = true;
    }

    bool UDPSocket::Bind(const string& s) {
        addr = s;
        cout << "Binding " << s << endl;
#ifdef _WIN32
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr(s.c_str());
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
        service.sin_addr.s_addr = htonl(INADDR_ANY);
        service.sin_port = htons(port);
        int result = bind(sckt, (struct sockaddr *) &service, sizeof (service));
        if (result < 0) {
            cerr << "Cannot bind port UDP " << port << endl;
            return false;
        }
#endif
        cout << "Bind sucessfully" << endl;
        return true;
    }

    bool operator==(const UDPSocket &one, const UDPSocket &other) {
        return one.GetAddress() == one.GetAddress();
    }

}
