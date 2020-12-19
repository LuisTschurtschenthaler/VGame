#include <SDL2/SDL.h>
#include "Timer.h"


Timer::Timer() {
	_time = static_cast<double>(SDL_GetTicks());
	_deltaTime = 0;
}

Timer::~Timer() {
}


void Timer::update() {
	double currentTime = static_cast<double>(SDL_GetTicks());
	_deltaTime = currentTime - _time;
	_time = currentTime;
}

double Timer::elapse() {
	double currentTime = static_cast<double>(SDL_GetTicks());
	return ((currentTime - _time) / 1000);
}
