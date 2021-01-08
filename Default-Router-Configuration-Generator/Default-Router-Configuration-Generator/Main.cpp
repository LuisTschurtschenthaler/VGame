#include <iostream>
#include "Input.h"
#include "RouterConfig.h"
#include "File.h"


int main() {

	do {
		RouterConfig config;
		config.hostname = Input::get("Hostname: ");
		config.bannerMotd = Input::get("Banner motd: ");

		Interface ena;
		ena.name = "ena";
		ena.password = Input::get("Enable password: ");
		ena.encryptPassword = Input::getYesOrNo("Should encrypt password (y/n): ");

		Interface con;
		con.name = "con";
		con.password = Input::get("Console password: ");
		con.encryptPassword = Input::getYesOrNo("Should encrypt password (y/n): ");
		con.timeoutMinutes = Input::getNumber("Exec timeout (minutes): ");
		con.timeoutSeconds = Input::getNumber("Exec timeout (seconds): ");

		Interface telnet;
		telnet.name = "vty";
		telnet.password = Input::get("Telnet password: ");
		telnet.encryptPassword = Input::getYesOrNo("Should encrypt password (y/n): ");
		telnet.timeoutMinutes = Input::getNumber("Exec timeout (minutes): ");
		telnet.timeoutSeconds = Input::getNumber("Exec timeout (seconds): ");
		telnet.userMin = Input::getNumber("User min: ");
		telnet.userMax = Input::getNumber("User max: ");

		bool skipAux = Input::getYesOrNo("Should skip aux configuration? (y/n): ");

		Interface aux;
		aux.name = "aux";
		if(skipAux) {
			aux.password = "default";
			aux.encryptPassword = false;
			aux.timeoutMinutes = 0;
			aux.timeoutSeconds = 0;
		}
		else {
			aux.password = Input::get("Aux password: ");
			aux.encryptPassword = Input::getYesOrNo("Should encrypt password (y/n): ");
			aux.timeoutMinutes = Input::getNumber("Exec timeout (minutes): ");
			aux.timeoutSeconds = Input::getNumber("Exec timeout (seconds): ");
		}

		config.interfaces.insert(config.interfaces.begin(), {
			ena, con, telnet, aux
		});

		std::cout << File::saveSolutions(config) << " saved" << std::endl;
		std::cin.ignore();
	} while(true);

	return 0;
}
