#include <iostream>
#include <algorithm>
#include <cctype>
#include "Input.h"

std::regex Input::numberRegex = std::regex("^[0-9]+$");

std::string Input::get(const std::string& message) {
	std::string input = "";

	do {
		clearConsole();
		std::cout << message;
		std::getline(std::cin, input);
	} while(input == "");

	return input;
}

bool Input::getYesOrNo(const std::string& message) {
	std::string input = "";

	do {
		clearConsole();
		std::cout << message;
		std::getline(std::cin, input);
		input = _stringTolower(input);
	} while(input != "y" && input != "n");

	return (input == "y");
}

int Input::getNumber(const std::string& message) {
	std::string input = "";

	do {
		clearConsole();
		std::cout << message;
		std::getline(std::cin, input);
	} while(!std::regex_match(input, numberRegex));

	return std::stoi(input.c_str());
}

void Input::clearConsole() {
#if defined _WIN32 || _WIN64
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}


std::string Input::_stringTolower(std::string string) {
	std::transform(string.begin(), string.end(), string.begin(),
				   [](unsigned char c) { 
			return std::tolower(c); 
		}
	);
	return string;
}
