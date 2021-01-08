#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <string>

class RouterConfig;

class File {

public:
	static const std::string DEFAULT_SAVE_FOLDER;


public:
	static std::string saveSolutions(const RouterConfig& config);

private:
	static void _outputInterfaces(std::ofstream& file, const RouterConfig& config, bool shouldEncryptpassword);
	static std::string _getTime();

};

#endif // FILE_H
