#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <map>
#include "Interface.h"

class Config {

public:
	std::string hostname, bannerMotd;
	std::vector<Interface> interfaces;

};

#endif // CONFIG_H
