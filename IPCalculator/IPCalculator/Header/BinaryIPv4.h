#ifndef BINARY_IP_V_4
#define BINARY_IP_V_4

#include <bitset>
#include <vector>

class BinaryIPv4 {

private:
	std::bitset<32> _binaryIP, _binarySubnetmask;

public:
	std::bitset<32> binaryNetworkID, binaryBroadcast;


public:
	BinaryIPv4(const std::vector<int>& ip, const std::vector<int>& mask);
	
	static std::bitset<32> getVectorInBinary(const std::vector<int>& vec);
	static std::vector<int> addOneToLastVariableOctet(const std::bitset<32>& bitset, const int fixOctets);
	static std::vector<int> addOneToLastVariableOctet(const std::bitset<32>& bitset);

private:
	std::bitset<32> _getBinaryNetworkID();
	std::bitset<32> _getBinaryBroadcast();

};

#endif // BINARY_IP_V_4
