#include <SDL2/SDL.h>
#include <GLEW/GL/glew.h>
#include "Window.h"
#include "SDLHandler.h"


int Window::_width = 0;
int Window::_height = 0;
bool Window::_fullscreen = false;
std::string Window::_title = "";


void Window::create(int width, int height, bool fullscreen, bool vSync, const std::string& title) {
	_width = width;
	_height = height;
	_fullscreen = fullscreen;
	_title = title;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLcreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullscreen, vSync);
	
	GLenum res = glewInit();
	if(res != GLEW_OK)
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
}

void Window::render() {
	swapBuffers();
	glFlush();
}

void Window::dispose() {
	destroyWindow();
	SDL_Quit();
}

bool Window::shouldClose() {
	return getCloseRequested();
}

void Window::setFullscreen() {
	_fullscreen = !_fullscreen;
	SDLsetFullscreen(_fullscreen);
}

void Window::setWindowSize(int width, int height) {
	SDLSetWindowSize(width, height);
	glViewport(0, 0, width, height);
	_width = width;
	_height = height;
}
