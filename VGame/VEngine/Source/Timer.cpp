#include "Timer.h"
#include <string>
#include <iostream>

double Timer::_last = 0.f;
double Timer::_delta = 0.f;

void Timer::setup() {
	_last = static_cast<float>(SDL_GetTicks());
	_delta = 0;
}

void Timer::update() {
	double now = static_cast<float>(SDL_GetTicks());
	_delta = now - _last;
	_last = now;
}
