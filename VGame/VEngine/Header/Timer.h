#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

class Timer {

private:
	static float _last;
	static float _delta;


public:
	static void setup();
	static void update();

	static float getTime() { return _last; }
	static float getDelta() { return _delta; }

};

#endif // TIMER_H
