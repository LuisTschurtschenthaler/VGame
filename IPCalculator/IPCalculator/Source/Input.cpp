#include <iostream>
#include <cctype>
#include "Input.h"
#include "Output.h"
#include "Util.h"

std::regex Input::_regexSubnet = std::regex("^[a-zA-Z-]+ *?([2-9]|[1-8][0-9]|9[0-9]|[1-8][0-9]{2}|9[0-8][0-9]|99[0-9]|10[01][0-9]|102[0-4])$");
std::regex Input::_regexIPAdress = std::regex("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
std::regex Input::_regexSubnetmask = std::regex("^(1[5-9]|2[0-9]|30)$");
std::regex Input::_regexContinue = std::regex("^[ynYN]$");


std::string Input::getIPv4Address() {
	std::string input = "";

	do {
		clearConsole();
		input = Output::printGetIPv4Address();
		
	} while(!std::regex_match(input, _regexIPAdress));

	return input;
}

/* Add support for IP-Format subnetmask */
std::string Input::getSubnetmask() {
	std::string input = "";

	do {
		clearConsole();
		input = Output::printGetSubnetmask();

	} while(/*!std::regex_match(input, _regexIPAdress) && */!std::regex_match(input, _regexSubnetmask));

	return input;
}

std::vector<std::string> Input::getSubnet(const std::vector<pair>& currentSubnets) {
	std::vector<std::string> result;
	std::string input = "";
	bool isValid = true;

	do {
		clearConsole();
		input = Output::printGetSubnet();

		if(std::regex_match(input, _regexSubnet)) {
			size_t pos = input.find(' ');
			std::string name = input.substr(0, pos);
			input.erase(0, pos + 1);

			for(const pair& subnet : currentSubnets) {
				if(Util::stringTolower(name) == Util::stringTolower(subnet.first)) {
					std::cout << "Name already taken! Press any key to continue..." << std::endl;
					std::cin.ignore();

					isValid = false;
					break;
				}
			}

			if(isValid) result = { name, input };
		}

	} while(!isValid);
	
	return result;
}

bool Input::isFinished() {
	std::string input = "";

	do {

		clearConsole();
		input = Output::printIsFinished();

	} while(!std::regex_match(input, _regexContinue));

	return !(Util::stringTolower(input) == "y");
}

bool Input::saveResultsToFile() {
	std::string input = "";

	do {

		clearConsole();
		input = Output::printShouldSaveFile();

	} while(!std::regex_match(input, _regexContinue));

	return (Util::stringTolower(input) == "y");
}

void Input::clearConsole() {
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}
