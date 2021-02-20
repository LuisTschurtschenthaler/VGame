#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <GLEW/GL/glew.h>
#include <STB/stb_image_write.h>
#include <ctime>
#include <iostream>
#include "Screenshot.h"
#include "Constants.h"
#include "Input.h"
#include "Log.h"


void Screenshot::handle() {
	if(Input::isKeyPressedAndReleased(GLFW_KEY_F2)) {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		int x = viewport[0],
			y = viewport[1];

		int width = viewport[2],
			height = viewport[3];

		char* imageData = (char*) malloc((size_t) (width * height * 3));
		if(!imageData) return;

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		_invertImage(imageData, width, height);

		std::string filename = PATH_SCREENSHOTS + _getScreenshotFilename();
		stbi_write_png(filename.c_str(), width, height, 3, imageData, 0);

		free(imageData);
	}
}


void Screenshot::_invertImage(char* imageData, const int& width, const int& height) {
	char rgb[3];

	for(int y = 0; y < height / 2; y++) {
		for(int x = 0; x < width; x++) {
			int top = (x + y * width) * 3;
			int bottom = (x + (height - y - 1) * width) * 3;

			memcpy(rgb, imageData + top, sizeof(rgb));
			memcpy(imageData + top, imageData + bottom, sizeof(rgb));
			memcpy(imageData + bottom, rgb, sizeof(rgb));
		}
	}
}

std::string Screenshot::_getScreenshotFilename() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H.%M.%S.jpg", timeinfo);
	std::string str(buffer);
	return str;
}
