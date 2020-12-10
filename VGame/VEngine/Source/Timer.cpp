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

#include <iostream>
double Timer::elapse() {
	double currentTime = static_cast<double>(SDL_GetTicks());
	//std::cout << (currentTime - _time) << std::endl;
	return ((currentTime - _time) / 1000);
}
