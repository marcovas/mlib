/*
 * NetworkInterface.h
 *
 *  Created on: 21 de ago de 2016
 *      Author: marcovas
 */

#ifndef NETWORKINTERFACE_H_
#define NETWORKINTERFACE_H_

#include <vector>
#include <string>

namespace mlib {

enum NetwotkInterfaceType {
	IF_UNKNOWN, IF_IPV4, IF_IPV6
};

class NetworkInterface {
public:
	static std::vector<NetworkInterface> GetNetworkInterfaces();
	std::string GetInterfaceName() const;
	std::string GetLocalAddress() const;
	NetwotkInterfaceType GetType() const;
	virtual ~NetworkInterface();
private:
	NetworkInterface();
	std::string ifName;
	std::string ip;
	NetwotkInterfaceType type;
};

}

#endif /* NETWORKINTERFACE_H_ */
