#include "SDLHandler.h"
#include "Window.h"
#include "Input.h"


static SDL_Window* window;
static SDL_GLContext context;
static bool isCloseRequested = false;


bool getCloseRequested() {
	return isCloseRequested;
}

void setCloseRequested(bool value) {
	isCloseRequested = value;
}

void SDLcreateWindow(const char* title, int x, int y, int width, int height, bool fullscreen) {
	int mode = 0;
	if(fullscreen) mode = SDL_WINDOW_FULLSCREEN;
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 
							  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | /*SDL_WINDOW_RESIZABLE |*/ mode);

	context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);
}

void swapBuffers() {
	SDL_GL_SwapWindow(window);
}

void SDLsetFullscreen(bool setFullscreen) {
	SDL_SetWindowFullscreen(window, setFullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void destroyWindow() {
	if(!window) return;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void SDLSetMousePosition(int x, int y) {
	SDL_WarpMouseInWindow(window, x, y);
}

void SDLSetWindowSize(int width, int height) {
	SDL_SetWindowSize(window, width, height);
}
