#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class Timer {

private:
	static double _last;
	static double _delta;


public:
	static void setup();
	static void update();

	static double getTime() { return _last; }
	static double getDelta() { return _delta; }

};

#endif // TIMER_H
