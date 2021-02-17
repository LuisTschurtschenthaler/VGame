#define _CRT_SECURE_NO_WARNINGS
#include "Screenshot.h"
#include "Window.h"
#include "Log.h"
#include <ctime>
#include <iostream>
#include <GLEW/GL/glew.h>

#define SDL_LOCKIFMUST(s) (SDL_MUSTLOCK(s) ? SDL_LockSurface(s) : 0)
#define SDL_UNLOCKIFMUST(s) { if(SDL_MUSTLOCK(s)) SDL_UnlockSurface(s); }

Key Screenshot::key = Key(KeyCode::KEY_F2);

bool Screenshot::take() {
	/*
	int w = Window::getWidth();
	int h = Window::getHeight();
	unsigned char* pixels = new unsigned char[w * h * 4];

	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, w, h, 8 * 4, w * 4, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

	std::string file =  _getScreenshotFilename();
	std::string path = "./Screenshots/" + file;

	if(_invert(surface)) {
		SDL_SaveBMP(surface, path.c_str());
		Log::write(Log::INFO, "Successfully saved screenshot: " + file);
	} else {
		Log::write(Log::WARN, "Failed to save screenshot: " + file);
		return false;
	}

	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	delete[] pixels;
	*/
	return true;
}

void Screenshot::handle() {
	key.update();

	if(key.wasPressedAndReleased())
		take();
}
/*
bool Screenshot::_invert(SDL_Surface* surface) {
	Uint8* t;
	Uint8* last;
	register Uint8* a;
	register Uint8* b;
	register Uint16 pitch;

	if(SDL_LOCKIFMUST(surface) < 0)
		return false;

	if(surface->h < 2) {
		SDL_UNLOCKIFMUST(surface);
		return true;
	}

	pitch = surface->pitch;
	t = (Uint8*) malloc(pitch);

	if(t == NULL) {
		SDL_UNLOCKIFMUST(surface);
		return false;
	}

	memcpy(t, surface->pixels, pitch);

	a = (Uint8*) surface->pixels;
	last = a + pitch * (surface->h - 1);
	b = last;

	while(a < b) {
		memcpy(a, b, pitch);
		a += pitch;
		memcpy(b, a, pitch);
		b -= pitch;
	}

	memmove(b, b + pitch, last - b);
	memcpy(last, t, pitch);

	free(t);
	SDL_UNLOCKIFMUST(surface);
	return true;
}*/

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