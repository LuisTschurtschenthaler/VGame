#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>
#include <map>


struct Interface {
	std::string name;
	std::string password;
	bool encryptPassword;

	uint8_t userMin, userMax;
	uint8_t timeoutMinutes, timeoutSeconds;
};


class RouterConfig {

public:
	std::string hostname, bannerMotd;
	std::vector<Interface> interfaces;

};

#endif // CONFIG_H
