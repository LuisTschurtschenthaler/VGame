#ifndef FRAMERATE_COUNTER_H
#define FRAMERATE_COUNTER_H


class FramerateCounter {

private:
	static int _frames;
	static double _startTime;
	static float _fps;


public:
	static void init();
	static void update();
	static const float getFPS() { return _fps; }

};

#endif // FRAMERATE_COUNTER_H
