#include <iostream>
#include "Constants.h"
#include "BinaryIPv4.h"
#include "IPv4.h"
#include "Util.h"


BinaryIPv4::BinaryIPv4(const std::vector<int>& ip, const std::vector<int>& mask) {
	_binaryIP			= getVectorInBinary(ip);
	_binarySubnetmask	= getVectorInBinary(mask);

	binaryNetworkID = _getBinaryNetworkID();
	binaryBroadcast = _getBinaryBroadcast();
}


std::vector<int> BinaryIPv4::addOneToLastVariableOctet(const std::bitset<32>& bitset, const int fixOctets) {
	std::vector<int> address = Util::binaryVectorToDecimal(bitset);
	
	if(address[3] < 255)
		address[3]++;
	else if(address[2] < 255)
		address[2]++;
	else if(address[1] < 255)
		address[1]++;
	else if(address[0] < 255)
		address[0]++;

	else if(address[2] < 255 && address[3] == 255) {
		address[3] = 0;
		address[2]++;
	}
	else if(address[1] < 255 && address[2] == 255) {
		address[2] = 0;
		address[1]++;
	}
	else if(address[0] < 255 && address[1] == 255) {
		address[1] = 0;
		address[0]++;
	}

	return address;
}

std::vector<int> BinaryIPv4::addOneToLastVariableOctet(const std::bitset<32>& bitset) {
	return Util::binaryVectorToDecimal(std::bitset<32>(bitset.to_ullong() + 1));
}

std::bitset<32> BinaryIPv4::getVectorInBinary(const std::vector<int>& vec) {
	std::bitset<32> binaryVector;

	for(size_t i = 0; i < AMOUNT_OF_OCTETS; i++)
		binaryVector |= std::bitset<32>(vec[i] << OCTET_MASK_BITSHIFTS[i]) & OCTET_MASKS[i];

	return binaryVector;
}


std::bitset<32> BinaryIPv4::_getBinaryNetworkID() {
	return std::bitset<32>(_binaryIP & _binarySubnetmask);
}

std::bitset<32> BinaryIPv4::_getBinaryBroadcast() {
	return std::bitset<32>(binaryNetworkID | (~_binarySubnetmask));
}
