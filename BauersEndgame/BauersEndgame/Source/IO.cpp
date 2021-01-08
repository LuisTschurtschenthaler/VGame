#include <iostream>
#include <conio.h>
#include "IO.h"
#include "Color.h"


KeyCode Input::readKey() {
	return static_cast<KeyCode>(_getch());
}



std::string Output::_foregroundColor = FG_COL_WHITE;
std::string Output::_backgroundColor = BG_COL_BLACK;


void Output::write(const std::string& message) {
	std::cout << _foregroundColor << _backgroundColor << message << COLOR_RESET;
}

void Output::writeLine(const std::string& message) {
	std::cout << _foregroundColor << _backgroundColor << message << COLOR_RESET << std::endl;
}

void Output::resetColors() {
	_foregroundColor = FG_COL_WHITE;
	_backgroundColor = BG_COL_BLACK;
}
