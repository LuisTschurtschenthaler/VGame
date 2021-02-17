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
		_startTime = 0;
		_first = false;
		return;
	}

	double timePassed = 0;
	if(timePassed - _startTime > 1000) {
		_fps = static_cast<float>(_frames / (timePassed - _startTime));
		_startTime = timePassed;
		_frames = 0;
	}
	_frames++;
}
