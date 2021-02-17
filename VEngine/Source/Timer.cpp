#include <GLFW/glfw3.h>
#include "Timer.h"


Timer::Timer() {
	_time = glfwGetTime();
	_deltaTime = 0;
}

Timer::~Timer() {
}


void Timer::update() {
	double currentTime = glfwGetTime();
	_deltaTime = currentTime - _time;
	_time = currentTime;
}

double Timer::elapse() {
	double currentTime = glfwGetTime();
	return (currentTime - _time);
}
