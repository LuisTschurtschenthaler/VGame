#ifndef INPUT_H
#define INPUT_H

#include <regex>
#include <string>
#include <vector>
#include "Util.h"

class Input {

private:
	static std::regex _regexSubnet, _regexIPAdress, _regexSubnetmask, _regexContinue;


public:
	static std::string getIPv4Address();
	static std::string getSubnetmask();
	static std::vector<std::string> getSubnet(const std::vector<pair>& currentSubnets);
	static bool isFinished();
	static bool saveResultsToFile();
	static void clearConsole();

};

#endif // INPUT_H
