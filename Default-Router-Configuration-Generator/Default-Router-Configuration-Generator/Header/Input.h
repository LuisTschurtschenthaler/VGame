#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <regex>

class Input {

private:
	static std::regex numberRegex;

public:
	static std::string get(const std::string& message = "");
	static bool getYesOrNo(const std::string& message = "");
	static int getNumber(const std::string& message = "");

	static void clearConsole();

private:
	static std::string _stringTolower(std::string string);

};

#endif // INPUT_H
