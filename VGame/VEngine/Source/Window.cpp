#include "Window.h"
#include "SDLHandler.h"
#include <GLEW/GL/glew.h>
#include <SDL2/SDL.h>

int Window::_width = 0;
int Window::_height = 0;
std::string Window::_title = "";


void Window::create(int width, int height, const std::string& title) {
	_width = width;
	_height = height;
	_title = title;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDLcreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, false);
	
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

float Window::getAspect() {
	return static_cast<float>(_width) / static_cast<float>(_height);
}

void Window::setWindowHeight(int width, int height) {
	SDLSetWindowSize(width, height);
	glViewport(0, 0, width, height);
	_width = width;
	_height = height;
}
