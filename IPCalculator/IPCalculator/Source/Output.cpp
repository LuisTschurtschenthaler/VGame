#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Output.h"
#include "IPv4.h"
#include "Input.h"
#include "Colors.h"
#include "Constants.h"


std::vector<std::string> Output::_delimiter = { "&0", "&1", "&2", "&3", "&4", "&5", "&6", "&7", "&8", 
												"&a", "&b", "&c", "&d", "&e", "&f", "&g", "&h", "&i" };


std::string Output::printGetIPv4Address() {
	std::string input = "";

	Input::clearConsole();
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################# &bAddress &7#################");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cFormat&8:\t<IPv4-Address>");
	_printColoredLine(" &7### &cExample&8:\t192.168.0.15");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cAddress&8: \t", false);
	std::getline(std::cin, input);

	return input;
}

std::string Output::printGetSubnetmask() {
	std::string input = "";

	Input::clearConsole();
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################### &bMask &7##################");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cFormat&8:\t<Subnetmask (15 - 30)>");
	_printColoredLine(" &7### &cExample&8:\t24");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cSubnetmask&8: ", false);
	std::getline(std::cin, input);

	return input;
}

std::string Output::printGetSubnet() {
	std::string input = "";

	Input::clearConsole();
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################# &bSubnet &7##################");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cFormat&8:\t<Name> <Hosts (2 - 1024)>");
	_printColoredLine(" &7### &cExample&8:\tInnichen 500");
	_printColoredLine(" &7### &cExample&8:\tInnichen-Bruneck 2");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cSubnet&8: \t", false);
	std::getline(std::cin, input);

	return input;
}

std::string Output::printIsFinished() {
	std::string input = "";

	Input::clearConsole();
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################# &bSubnet &7##################");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cFormat&8:\t\t<&cy&8/&bn&8>");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cAnother subnet?&8: \t", false);
	std::getline(std::cin, input);

	return input;
}

void Output::printGeneral(const IPv4& network) {
	Input::clearConsole();
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################# &bNetwork &7#################");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cIP-Address&8:\t" + _formatFinalIP(network.ip, network.fixOctets) + "/" + std::to_string(static_cast<int>(network.CIDR)));
	_printColoredLine(" &7### &cSubnetmask&8:\t" + Util::vectorToString(network.subnetmask));
	_printColoredLine(" &7### &cWildcard&8:\t\t" + Util::vectorToString(network.wildcard));
	_printColoredLine(" &7### &cClass&8:\t\t" + Util::getClassInformation(network.ip));
}

void Output::printDetail(const IPv4& network) {
	int fix = network.fixOctets;
	int hosts = IPv4::getAmountOfHosts(network.CIDR);
	std::string hostsFit = ((hosts >= network.usedHosts) ? "&cYES" : "&bNO");

	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################# &bDetails &7#################");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cSubnetmask&8:\t" + Util::vectorToString(network.subnetmask));
	_printColoredLine(" &7### &cNetwork-ID&8:\t" + _formatFinalIP(network.networkID, fix) + "/" + std::to_string(static_cast<int>(network.CIDR)));
	_printColoredLine(" &7### &cFirst host&8:\t" + _formatFinalIP(network.firstHost, fix));
	_printColoredLine(" &7### &cLast host&8:\t\t" + _formatFinalIP(network.lastHost, fix));
	_printColoredLine(" &7### &cBroadcast&8:\t\t" + _formatFinalIP(network.broadcast, fix));
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cUsed/Hosts&8:\t" + std::to_string(network.usedHosts) + "/" + std::to_string(hosts));
	_printColoredLine(" &7### &cSubnets fit&8:\t" + hostsFit);
	_printColoredLine(" &7### &cSubnets Am.&8:\t" + std::to_string(network.amountOfSubnets));
	_printColoredLine(" &7###########################################");
	std::cout << std::endl;
}

void Output::printSubnets(const std::vector<std::pair<std::string, IPv4*>>& subnets, int fixOctets) {
	if(subnets.size() <= 0) return;
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################# &bSubnets &7#################");
	_printColoredLine(" &7###########################################");

	for(auto& subnet : subnets) {
		int hosts = subnet.second->getAmountOfHosts(subnet.second->CIDR);

		_printColoredLine(" &7### &cSubnetname&8:\t" + subnet.first);
		_printColoredLine(" &7### &cSubnetmask&8:\t" + Util::vectorToString(subnet.second->subnetmask));
		_printColoredLine(" &7### &cWildcard&8:\t\t" + Util::vectorToString(IPv4::getWilrdcard(subnet.second->subnetmask)));
		_printColoredLine(" &7### &cNetwork-ID&8:\t" + _formatFinalIP(subnet.second->networkID, fixOctets) + "/" + std::to_string(static_cast<int>(subnet.second->CIDR)));
		_printColoredLine(" &7### &cFirst host&8:\t" + _formatFinalIP(subnet.second->firstHost, fixOctets));
		_printColoredLine(" &7### &cLast host&8:\t\t" + _formatFinalIP(subnet.second->lastHost, fixOctets));
		_printColoredLine(" &7### &cBroadcast&8:\t\t" + _formatFinalIP(subnet.second->broadcast, fixOctets));
		_printColoredLine(" &7### &cFree Hosts&8:\t" + std::to_string(hosts) + "/" + std::to_string(hosts + ADDITIONAL_REQUIRED_HOSTS));
		_printColoredLine(" &7###########################################");
	}
}

std::string Output::printShouldSaveFile() {
	std::string input = "";

	Input::clearConsole();
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7################## &bSave? &7##################");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cFormat&8:\t\t<&cy&8/&bn&8>");
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cSave results?&8: \t", false);
	std::getline(std::cin, input);

	return input;
}

void Output::printFileSaved(const std::string& path) {
	Input::clearConsole();
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cFile successfully saved to&8:");
	_printColoredLine(" &7### &b" + path);
	_printColoredLine(" &7###########################################");
	_printColoredLine(" &7### &cPress any key to continue...");
	_printColoredLine(" &7###########################################");
	std::cin.ignore();
}


std::string Output::_formatFinalIP(std::vector<int> ip, int fixOctets) {
	std::string result = UNDERLINED_WHITE;

	for(size_t i = 0; i < AMOUNT_OF_OCTETS; i++) {
		result += std::to_string(ip[i]);

		if(i < 3) result += ".";
		if(i == fixOctets - 1) result += RESET;
	}
	return result;
}

void Output::_printColoredLine(std::string input, bool endl) {
	std::string finalString = "";
	size_t start = 0;

	for(auto& delimiter : _delimiter) {
		while((start = input.find(delimiter)) != std::string::npos)
			input.replace(start, delimiter.length(), RESET + _getColor(delimiter));
	}

	std::cout << input << RESET;
	if(endl) std::cout << std::endl;
}

std::string Output::_getColor(std::string delimiter) {
	switch(_hash(delimiter.c_str())) {
		case _hash("&0"): return RESET;
		case _hash("&1"): return BLACK;
		case _hash("&2"): return RED;
		case _hash("&3"): return GREEN;
		case _hash("&4"): return YELLOW;
		case _hash("&5"): return BLUE;
		case _hash("&6"): return MAGENTA;
		case _hash("&7"): return CYAN;
		case _hash("&8"): return WHITE;
		case _hash("&a"): return BOLD_BLACK;
		case _hash("&b"): return BOLD_RED;
		case _hash("&c"): return BOLD_GREEN;
		case _hash("&d"): return BOLD_YELLOW;
		case _hash("&e"): return BOLD_BLUE;
		case _hash("&f"): return BOLD_MAGENTA;
		case _hash("&g"): return BOLD_CYAN;
		case _hash("&h"): return BOLD_WHITE;
		case _hash("&i"): return UNDERLINED_WHITE;
		default: return delimiter;
	}
}
