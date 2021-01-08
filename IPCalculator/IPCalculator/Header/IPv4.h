#ifndef IP_V_4
#define IP_V_4

#include <string>
#include <vector>
#include "BinaryIPv4.h"

class IPv4 {

public:
	std::vector<std::pair<std::string, IPv4*>> subnets;

	int CIDR, fixOctets, usedHosts, amountOfSubnets;
	std::vector<int> ip, subnetmask, wildcard;
	std::vector<int> networkID, firstHost, broadcast, lastHost;

private:
	BinaryIPv4* _binaryIPv4;


public:
	IPv4(const std::string& address, std::string mask);
	IPv4(std::vector<int> networkID, int CIDR);

	void addSubnet(const std::string& name, const int size);
	void print();

	static std::vector<int> getWilrdcard(const std::vector<int>& subnetmask);
	static int getAmountOfHosts(int CIDR);

private:
	std::vector<int> _getNetworkID();
	std::vector<int> _getFirstHost();

	std::vector<int> _getBroadcast();
	std::vector<int> _getLastHost();

};

#endif // IP_V_4
