#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include "Util.h"
#include "IPv4.h"
#include "Constants.h"


std::string Util::stringTolower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}

std::vector<int> Util::getIPAdressFromString(const std::string& address) {
	std::stringstream ss(address);
	std::vector<int> octets;

	std::string temp;
	while(std::getline(ss, temp, '.'))
		octets.push_back(std::atoi(temp.c_str()));

	return octets;
}

std::vector<int> Util::getSubnetmaskFromString(const std::string& mask) {
	int subnetmask = 32 - std::stoi(mask);
	std::string finalSubnetmask = "";

	int result = 0;
	if(subnetmask >= 0 && subnetmask <= 8) {
		for(int i = 0; i < subnetmask; i++)
			result += static_cast<int>(pow(2, i));

		finalSubnetmask = "255.255.255." + std::to_string(255 - result);
	}

	else if(subnetmask >= 8 && subnetmask <= 16) {
		subnetmask = 16 - subnetmask;
		subnetmask = 8 - subnetmask;

		for(int i = 0; i < subnetmask; i++)
			result += static_cast<int>(pow(2, i));

		finalSubnetmask = "255.255." + std::to_string(255 - result) + ".0";
	}

	else if(subnetmask >= 16 && subnetmask <= 24) {
		subnetmask = 24 - subnetmask;
		subnetmask = 8 - subnetmask;

		for(int i = 0; i < subnetmask; i++)
			result += static_cast<int>(pow(2, i));

		finalSubnetmask = "255." + std::to_string(255 - result) + ".0.0";
	}

	else {
		subnetmask = 32 - subnetmask;
		subnetmask = 8 - subnetmask;

		for(int i = 0; i < subnetmask; i++)
			result += static_cast<int>(pow(2, i));

		finalSubnetmask = std::to_string(255 - result) + ".0.0.0";
	}

	return getIPAdressFromString(finalSubnetmask);
}

std::string Util::vectorToString(const std::vector<int>& vec) {
	std::string result = "";

	for(size_t i = 0; i < AMOUNT_OF_OCTETS; i++) {
		result += std::to_string(vec[i]);

		if(i < 3) result += ".";
	}

	return result;
}

std::vector<int> Util::binaryVectorToDecimal(const std::bitset<32>& bits) {
	std::vector<int> decimals;

	std::stringstream address;
	address << _bitsetToString(bits);

	std::string temp;
	while(getline(address, temp, '.'))
		decimals.push_back(std::atoi(temp.c_str()));

	return decimals;
}

std::string Util::getClassInformation(const std::vector<int>& ip) {
	if(ip[0] == 10)
		return "'A', private block";

	else if(ip[0] == 172 && ip[1] >= 16 && ip[1] <= 31)
		return "'B', private block";

	else if(ip[0] == 192 && ip[1] == 168)
		return "'C', private block";

	else if(ip[0] == 127)
		return "Reserved block, System Loopback Address";

	else if(ip[0] >= 0 && ip[0] < 127)
		return "A";

	else if(ip[0] > 127 && ip[0] < 192)
		return "B";

	else if(ip[0] > 191 && ip[0] < 224)
		return "C";

	else if(ip[0] > 223 && ip[0] < 240)
		return "D, Multicast IP-Address block";

	else if(ip[0] > 239 && ip[0] <= 255)
		return "E, Multicast IP-Address block";

	else return "Not in range";
}


std::string Util::_bitsetToString(const std::bitset<32>& bits) {
	std::string result = "";

	for(int i = 0; i <= AMOUNT_OF_DOTS; i++) {
		result += std::to_string(((bits & OCTET_MASKS[i]) >> OCTET_MASK_BITSHIFTS[i]).to_ulong());
		if(i != AMOUNT_OF_DOTS) result += ".";
	}

	return result;
}
