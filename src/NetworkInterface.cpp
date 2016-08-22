/*
 * NetworkInterface.cpp
 *
 *  Created on: 21 de ago de 2016
 *      Author: marcovas
 */

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <linux/if_link.h>
#endif

#include <iostream>

#include "NetworkInterface.h"

namespace mlib {

NetworkInterface::NetworkInterface() :
		ifName(""), ip(""), type(IF_UNKNOWN) {
}

#ifdef _WIN32
std::vector<NetworkInterface> NetworkInterface::GetNetorkInterfaces() {
	std::vector<NetworkInterface> interfaces;
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		std::cerr << "Error " << WSAGetLastError()
				<< " when getting local host name." << std::endl;
		return interfaces;
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		std::cerr << "Bad host lookup." << std::endl;
		return interfaces;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		NetworkInterface ninterface;
		switch (phe->h_addrtype) {
		case AF_INET:
			ninterface.type = IF_IPV4;
			break;
		case AF_INET6:
			ninterface.type = IF_IPV6;
			break;
		default:
			continue;
		}
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		ninterface.ifName = std::string(phe->h_name);
		ninterface.ip = inet_ntoa(addr);
		interfaces.insert(interfaces.begin(), ninterface);
	}
	return interfaces;
}
#else
std::vector<NetworkInterface> NetworkInterface::GetNetorkInterfaces() {
	std::vector<NetworkInterface> interfaces;

	ifaddrs *ifaddr, *ifa;
	int family, s, n;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return interfaces;
	}

	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if (ifa->ifa_addr == NULL)
		continue;
		NetworkInterface ninterface;
		family = ifa->ifa_addr->sa_family;

		std::string interfaceName = std::string(ifa->ifa_name);
		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ?
					sizeof(struct sockaddr_in) :
					sizeof(struct sockaddr_in6), host, NI_MAXHOST,
					NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				std::cerr << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
				return interfaces;
			}
			std::string address = std::string(host);
			ninterface.ifName = interfaceName;
			ninterface.ip = host;
			if (family == AF_INET) {
				ninterface.type = IF_IPV4;
			} else {
				ninterface.type = IF_IPV6;
			}
			interfaces.insert(interfaces.begin(), ninterface);
		}
	}
	freeifaddrs(ifaddr);
	return interfaces;
}
#endif

NetwotkInterfaceType NetworkInterface::GetType() const {
	return type;
}

std::string NetworkInterface::GetInterfaceName() const {
	return ifName;
}

std::string NetworkInterface::GetLocalAddress() const {
	return ip;
}

NetworkInterface::~NetworkInterface() {
	// TODO Auto-generated destructor stub
}

}
