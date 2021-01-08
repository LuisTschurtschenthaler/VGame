#ifndef EVENT_H
#define EVENT_H

#include "EventTypes.h"


class IEvent {

public:
	virtual const EventType& getEventType() const = 0;

};


template<typename... Args>
class Event 
	: public IEvent {

public:
	using EventFunction = std::function<void(Args...)>;

private:
	const EventType& _eventType;
	const EventFunction _eventFunction;


public:
	Event(const EventType& eventType, const EventFunction& eventFunction)
		: _eventType(eventType), _eventFunction(eventFunction) { 
	}
	~Event() { }
	
	void handle(Args... args) { this->_eventFunction(args...); }

	const EventType& getEventType() const override { return _eventType; }

};

#endif // EVENT_H
