#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <bitset>

typedef std::pair<std::string, int> pair;

class Util {

public:
	static std::string stringTolower(std::string s);

	static std::vector<int> getIPAdressFromString(const std::string& address);
	static std::vector<int> getSubnetmaskFromString(const std::string& mask);

	static std::string vectorToString(const std::vector<int>& vec);
	static std::vector<int> binaryVectorToDecimal(const std::bitset<32>& bits);

	static std::string getClassInformation(const std::vector<int>& ip);

private:
	static std::string _bitsetToString(const std::bitset<32>& bits);

};

#endif // UTIL_H
