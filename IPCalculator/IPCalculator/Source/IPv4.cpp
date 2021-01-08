#include <math.h>
#include <iostream>
#include "IPv4.h"
#include "Util.h"
#include "Output.h"
#include "Constants.h"
#include "Input.h"


IPv4::IPv4(const std::string& address, std::string mask) 
	: CIDR(std::stoi(mask)), usedHosts(0), amountOfSubnets(0) {
	
	fixOctets = static_cast<int>(std::floor(CIDR / OCTET_SIZE));

	ip			= Util::getIPAdressFromString(address);
	subnetmask	= Util::getSubnetmaskFromString(mask);
	wildcard	= getWilrdcard(subnetmask);

	_binaryIPv4 = new BinaryIPv4(ip, subnetmask);

	networkID = _getNetworkID();
	firstHost = _getFirstHost();
	broadcast = Util::binaryVectorToDecimal(_binaryIPv4->binaryBroadcast);
	lastHost = _getLastHost();
}

IPv4::IPv4(std::vector<int> _networkID, int CIDR)
	: CIDR(CIDR), usedHosts(0), amountOfSubnets(0) {

	fixOctets = static_cast<int>(std::floor(CIDR / OCTET_SIZE));

	subnetmask = Util::getSubnetmaskFromString(std::to_string(CIDR));
	wildcard = getWilrdcard(subnetmask);

	_binaryIPv4 = new BinaryIPv4(_networkID, subnetmask);

	networkID = _networkID;
	firstHost = _getFirstHost();
	broadcast = _getBroadcast();
	lastHost = _getLastHost();
}


void IPv4::addSubnet(const std::string& name, const int size) {
	int nextCIDR = static_cast<int>(32 - (std::ceil(log(size + 1) / log(2))));
	std::vector<int> address = std::vector<int>();

	if(subnets.size() <= 0)
		address = networkID;
	else address = BinaryIPv4::addOneToLastVariableOctet(subnets[subnets.size() - 1].second->_binaryIPv4->binaryBroadcast);

	IPv4* subnet = new IPv4(address, nextCIDR);
	subnets.push_back(std::make_pair<>(name, subnet));

	usedHosts += getAmountOfHosts(nextCIDR) + ADDITIONAL_REQUIRED_HOSTS;
	amountOfSubnets++;
}

void IPv4::print() {
	Input::clearConsole();

	Output::printGeneral(*this);
	Output::printDetail(*this);
	Output::printSubnets(subnets, fixOctets);
}

int IPv4::getAmountOfHosts(int CIDR) {
	return static_cast<int>(std::pow(2, (32 - CIDR)) - ADDITIONAL_REQUIRED_HOSTS);
}


std::vector<int> IPv4::getWilrdcard(const std::vector<int>& subnetmask) {
	std::vector<int> wildcard;

	for(int i = 0; i < AMOUNT_OF_OCTETS; i++)
		wildcard.push_back(255 - subnetmask[i]);

	return wildcard;
}

std::vector<int> IPv4::_getNetworkID() {
	return Util::binaryVectorToDecimal(_binaryIPv4->binaryNetworkID);
}

std::vector<int> IPv4::_getFirstHost() {
	std::vector<int> address, finalAddress;
	bool addedOne = false;

	for(int i = AMOUNT_OF_OCTETS - 1; i >= 0; i--) {
		if(networkID[i] >= 0 && i >= fixOctets && !addedOne) {
			address.push_back(networkID[i] + 1);
			addedOne = true;
		}
		else address.push_back(networkID[i]);
	}

	for(int i = address.size() - 1; i >= 0; i--)
		finalAddress.push_back(address[i]);

	return finalAddress;
}

std::vector<int> IPv4::_getBroadcast() {
	std::vector<int> broadcast;
	int hosts = getAmountOfHosts(CIDR) + ADDITIONAL_REQUIRED_HOSTS;

	for(int i = 0; i < AMOUNT_OF_OCTETS; i++) {
		if(subnetmask[i] == 255)
			broadcast.push_back(networkID[i]);

		else if(subnetmask[i] == 0)
			broadcast.push_back(255);

		else if((networkID[i] + hosts) <= 256)
			broadcast.push_back(networkID[i] + hosts - 1);

		else {
			int multiplier = 256 - subnetmask[i];
			int count = 0;

			do {
				count += multiplier;
			} while(count < networkID[i]);

			std::cout << count << std::endl;
			broadcast.push_back(networkID[i] + count);
		}
	}
	return broadcast;
}

std::vector<int> IPv4::_getLastHost() {
	std::vector<int> address, finalAddress;
	bool removedOne = false;

	for(int i = AMOUNT_OF_OCTETS - 1; i >= 0; i--) {
		if(broadcast[i] <= 255 && i >= fixOctets && !removedOne) {
			address.push_back(broadcast[i] - 1);
			removedOne = true;
		}
		else address.push_back(broadcast[i]);
	}

	for(int i = address.size() - 1; i >= 0; i--)
		finalAddress.push_back(address[i]);

	return finalAddress;
}
