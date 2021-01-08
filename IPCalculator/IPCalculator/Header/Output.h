#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>
class IPv4;

class Output {

private:
	static std::vector<std::string> _delimiter;


public:
	static std::string printGetIPv4Address();
	static std::string printGetSubnetmask();
	static std::string printGetSubnet();
	static std::string printIsFinished();
	static std::string printShouldSaveFile();
	static void printFileSaved(const std::string& path);

	static void printGeneral(const IPv4& network);
	static void printDetail(const IPv4& network);
	static void printSubnets(const std::vector<std::pair<std::string, IPv4*>>& subnets, int fixOctets);

private:
	static std::string _formatFinalIP(std::vector<int> ip, int fixOctets);

	static void _printColoredLine(std::string input, bool endl = true);
	static std::string _getColor(std::string delimiter);

};

#endif // OUTPUT_H
