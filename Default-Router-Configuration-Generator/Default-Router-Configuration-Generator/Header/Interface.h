#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>

struct Interface {
	std::string name;
	std::string password;
	bool encryptPassword;

	uint8_t userMin, userMax;
	uint8_t timeoutMinutes, timeoutSeconds;
};

#endif // INTERFACE_H
