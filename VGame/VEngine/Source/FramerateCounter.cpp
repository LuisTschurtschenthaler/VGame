#include <SDL2/SDL.h>
#include "FramerateCounter.h"

int FramerateCounter::_frames = 0;
double FramerateCounter::_startTime = 0.0;
bool FramerateCounter::_first = 0;
float FramerateCounter::_fps = 0.f;


void FramerateCounter::init() {

}

void FramerateCounter::update() {
	if(_first) {
		_frames = 0;
		_startTime = SDL_GetTicks();
		_first = false;
		return;
	}

	double timePassed = SDL_GetTicks();
	if(timePassed - _startTime > 1000)
	{
		_fps = (double) _frames / (timePassed - _startTime);
		_startTime = timePassed;
		_frames = 0;
	}
	_frames++;
}
