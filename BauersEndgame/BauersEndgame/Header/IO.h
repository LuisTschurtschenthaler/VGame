#ifndef IO_H
#define IO_H

#include <string>
#include "KeyCode.h"

class Input {

public:
	static KeyCode readKey();

};


class Output {

private:
	static std::string _foregroundColor, _backgroundColor;

public:
	static void write(const std::string& message);
	static void writeLine(const std::string& message);
	static void resetColors();

	static void setForegroundColor(const std::string& newColor) { _foregroundColor = newColor; }
	static void setBackgroundColor(const std::string& newColor) { _backgroundColor = newColor; }

};


#endif // IO_H
