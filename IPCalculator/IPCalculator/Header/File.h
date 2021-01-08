#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <string>
#include <vector>
class IPv4;



class File {

public:
	static const std::string DEFAULT_SAVE_FOLDER;


public:
	static std::string saveSolutions(const IPv4& network);

private:
	static std::string _getNewFilename(const IPv4& network);
	static void _saveNetwork(std::ofstream& file, const IPv4& network);
	static void _saveDetails(std::ofstream& file, const IPv4& network);
	static void _saveSubnets(std::ofstream& file, const std::vector<std::pair<std::string, IPv4*>>& subnets);

};

#endif // FILE_H
