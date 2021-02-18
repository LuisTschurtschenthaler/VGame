#include <GLFW/glfw3.h>
#include "FramerateCounter.h"


int FramerateCounter::_frames = 0;
double FramerateCounter::_startTime = 0.0;
float FramerateCounter::_fps = 0.f;


void FramerateCounter::init() {
	_startTime = glfwGetTime();
}

void FramerateCounter::update() {
	double timePassed = glfwGetTime();
	if(timePassed - _startTime > 1) {
		_fps = static_cast<float>(_frames / (timePassed - _startTime));
		_startTime = timePassed;
		_frames = 0;
	}
	_frames++;
}
