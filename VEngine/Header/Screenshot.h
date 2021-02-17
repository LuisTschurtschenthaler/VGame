#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <string>
#include "Input.h"

class Screenshot {

private:
	static Key key;


public:
	static bool take();
	static void handle();

private:
	//static bool _invert(SDL_Surface* surface);
	static std::string _getScreenshotFilename();

};

#endif // SCREENSHOT_H