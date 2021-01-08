#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <map>
#include <string>
#include <vector>
#include <functional>
#include "Event.h"
#include "Log.h"

#include "BlockEvents.h"

class Camera;
class ChunkManager;

class EventDispatcher {

private:
	std::map<EventType, std::vector<IEvent*>> _events;


public:
	EventDispatcher() { }
	~EventDispatcher() { }


	static void registerEvents(EventDispatcher& dispatcher) {
		dispatcher.registerEvent(new Event<ChunkManager*>(EventType::BLOCK_BREAK_EVENT, &BlockEvents::onBlockBreak));
		dispatcher.registerEvent(new Event<ChunkManager*>(EventType::BLOCK_PLACE_EVENT, &BlockEvents::onBlockPlace));
	}

	void registerEvent(IEvent* e) {
		_events[e->getEventType()].push_back(e);
	}

	template<typename... Args>
	void dispatchEvent(const EventType& type, Args... args) {
		auto it = _events.find(type);

		if(it != _events.end()) {
			for(auto itEvent : it->second) {
				if(Event<Args...>* ev = dynamic_cast<Event<Args...>*>(itEvent))
					ev->handle(args...);
			}
		} else Log::write(Log::ERROR, "EventType not found!");
	}

};

#endif // EVENT_DISPATCHER_H
