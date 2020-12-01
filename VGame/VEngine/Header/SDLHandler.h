#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL2/SDL.h>

bool getCloseRequested();
void setCloseRequested(bool value);
void SDLcreateWindow(const char* title, int x, int y, int width, int height, bool fullscreen);
void swapBuffers();
void SDLsetFullscreen(bool value);
void destroyWindow();
void SDLSetMousePosition(int x, int y);
void SDLSetWindowSize(int width, int height);

#endif // SDL_HANDLER_H