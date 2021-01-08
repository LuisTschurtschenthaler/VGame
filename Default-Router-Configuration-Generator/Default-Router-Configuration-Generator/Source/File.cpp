#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
#include <experimental/filesystem>
#include "File.h"
#include "RouterConfig.h"


namespace fs = std::experimental::filesystem;
const std::string File::DEFAULT_SAVE_FOLDER = "./Configurations/";


std::string File::saveSolutions(const RouterConfig& config) {
	if(!fs::is_directory(DEFAULT_SAVE_FOLDER))
		fs::create_directory(DEFAULT_SAVE_FOLDER);

	std::ofstream file;
	std::string fileName = DEFAULT_SAVE_FOLDER + config.hostname + ".txt";
	file.open(fileName, std::ios::app);

	/* SAVE EVERYTHING IN FILE */

	file << "ena" << std::endl;
	file << "conf t" << std::endl;

	file << "hostname " << config.hostname << std::endl;
	file << "banner motd #" << config.bannerMotd << "#" << std::endl;
	file << "no ip domain-lookup" << std::endl;
	
	_outputInterfaces(file, config, true);
	file << "service password-encryption" << std::endl;
	file << "no service password-encryption" << std::endl;
	_outputInterfaces(file, config, false);

	file << "clock set " << _getTime() << std::endl;
	file << "exit" << std::endl;
	file << "copy run start" << std::endl;

	/* SAVE EVERYTHING IN FILE */

	file.close();
	return fileName;
}


void File::_outputInterfaces(std::ofstream& file, const RouterConfig& config, bool shouldEncryptpassword) {
	for(auto& interf : config.interfaces) {
		if(interf.encryptPassword != shouldEncryptpassword)
			continue;

		if(interf.name == "con" || interf.name == "aux") {
			file << "line " << interf.name << " 0" << std::endl;

			if(interf.name == "con")
				file << "logging synchronous" << std::endl;
		}

		else if(interf.name == "vty")
			file << "line vty " << std::to_string(interf.userMin) << " " << std::to_string(interf.userMax) << std::endl;

		else { // ena
			file << "ena secret " << interf.password << std::endl;
			continue;
		}

		file << "exec-timeout " << std::to_string(interf.timeoutMinutes) << " " << std::to_string(interf.timeoutSeconds) << std::endl;
		file << "password " << interf.password << std::endl;
		file << "login" << std::endl;
		file << "exit" << std::endl;
	}
}

std::string File::_getTime() {
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%H:%M:00 %d %h %Y", timeinfo);
	return std::string(buffer);
}
