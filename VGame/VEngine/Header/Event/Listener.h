#ifndef LISTENER_H
#define LISTENER_H

class Listener {

public:
	virtual ~Listener() { }

	virtual void handleEvent() = 0;

};

#endif // LISTENER_H
