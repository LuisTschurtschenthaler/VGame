#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
#include <experimental/filesystem>
#include "File.h"
#include "IPv4.h"
#include "Util.h"
#include "Constants.h"


namespace fs = std::experimental::filesystem;
const std::string File::DEFAULT_SAVE_FOLDER = "./Networks/";


std::string File::saveSolutions(const IPv4& network) {
	if(!fs::is_directory(DEFAULT_SAVE_FOLDER))
		fs::create_directory(DEFAULT_SAVE_FOLDER);

	std::ofstream file;
	std::string fileName = DEFAULT_SAVE_FOLDER + _getNewFilename(network);
	file.open(fileName, std::ios::app);

	_saveNetwork(file, network);
	_saveDetails(file, network);
	_saveSubnets(file, network.subnets);

	file.close();
	return fileName;
}


std::string File::_getNewFilename(const IPv4& network) {
	return std::string(Util::vectorToString(network.networkID) + " - " + std::to_string(network.CIDR) + ".txt");
	/*
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H.%M.%S.txt", timeinfo);
	return std::string(buffer);
	*/
}

void File::_saveNetwork(std::ofstream& file, const IPv4& network) {
	file << "Network: " << std::endl;
	file << "IP-Address: " + Util::vectorToString(network.ip) + "/" + std::to_string(static_cast<int>(network.CIDR)) << std::endl;
	file << "Subnetmask: " + Util::vectorToString(network.subnetmask) << std::endl;
	file << "Wildcard: " + Util::vectorToString(network.wildcard) << std::endl;
	file << "Class: " + Util::getClassInformation(network.ip) << std::endl;
	file << std::endl;
}

void File::_saveDetails(std::ofstream& file, const IPv4& network) {
	int hosts = IPv4::getAmountOfHosts(network.CIDR);
	std::string hostsFit = ((hosts >= network.usedHosts) ? "YES" : "NO");

	file << "Details:" << std::endl;
	file << "Subnetmask: " + Util::vectorToString(network.subnetmask) << std::endl;
	file << "Network-ID: " + Util::vectorToString(network.networkID) + "/" + std::to_string(static_cast<int>(network.CIDR)) << std::endl;
	file << "First host: " + Util::vectorToString(network.firstHost) << std::endl;
	file << "Last host: " + Util::vectorToString(network.lastHost) << std::endl;
	file << "Broadcast: " + Util::vectorToString(network.broadcast) << std::endl;
	file << "Used/Hosts: " + std::to_string(network.usedHosts) + "/" + std::to_string(hosts) << std::endl;
	file << "Subnets fit: " + hostsFit << std::endl;
	file <<"Subnets Am.: " + std::to_string(network.amountOfSubnets);
	file << std::endl << std::endl;
}

void File::_saveSubnets(std::ofstream& file, const std::vector<std::pair<std::string, IPv4*>>& subnets) {
	if(subnets.size() <= 0) return;
	file << "Subnets:" << std::endl;

	for(auto& subnet : subnets) {
		int hosts = subnet.second->getAmountOfHosts(subnet.second->CIDR);

		file << "Subnetname: " + subnet.first << std::endl;
		file << "Subnetmask: " + Util::vectorToString(subnet.second->subnetmask) << std::endl;
		file << "Wildcard: " + Util::vectorToString(IPv4::getWilrdcard(subnet.second->subnetmask)) << std::endl;
		file << "Network-ID: " + Util::vectorToString(subnet.second->networkID) + "/" + std::to_string(static_cast<int>(subnet.second->CIDR)) << std::endl;
		file << "First host: " + Util::vectorToString(subnet.second->firstHost) << std::endl;
		file << "Last host: " + Util::vectorToString(subnet.second->lastHost) << std::endl;
		file << "Broadcast: " + Util::vectorToString(subnet.second->broadcast) << std::endl;
		file << "Free Hosts: " + std::to_string(hosts) + "/" + std::to_string(hosts + ADDITIONAL_REQUIRED_HOSTS) << std::endl;
		file << std::endl;
	}
}
