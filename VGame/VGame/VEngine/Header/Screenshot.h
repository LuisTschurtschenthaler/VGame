#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <string>


class Screenshot {

public:
	static void handle();

private:
	static void _invertImage(char* imageData, const int& width, const int& height);
	static std::string _getScreenshotFilename();

};

#endif // SCREENSHOT_H