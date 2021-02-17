#ifndef TIMER_H
#define TIMER_H


class Timer {
	
private:
	double _time, _lastDeltaTime, _deltaTime;

public:
	Timer();
	~Timer();

	void update();
	double elapse();

	double const getDeltaTime() const { return _deltaTime; }

};

#endif // TIMER_H
