#include "Timer.h"

float Timer::_last = 0.f;
float Timer::_delta = 0.f;

void Timer::setup() {
	_last = static_cast<float>(SDL_GetTicks());
	_delta = 0;
}

void Timer::update() {
	float now = static_cast<float>(SDL_GetTicks());
	_delta = now - _last;
	_last = now;
}
