#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <map>
#include <string>
#include <vector>
#include <functional>
#include "Event.h"
#include "Log.h"

#include "BlockEvents.h"
#include "PlayerEvents.h"

class Camera;
class ChunkManager;


class EventDispatcher {

private:
	std::map<EventType, std::vector<IEvent*>> _events;


public:
	EventDispatcher() { }
	~EventDispatcher() { }


	static void registerEvents(EventDispatcher& dispatcher) {
		dispatcher.registerEvent(new Event<LocationXYZ, BlockID, BlockRotation>(EventType::BLOCK_PLACE_EVENT, &BlockEvents::onBlockPlace));
		dispatcher.registerEvent(new Event<LocationXYZ>(EventType::BLOCK_BREAK_EVENT, &BlockEvents::onBlockBreak));
		dispatcher.registerEvent(new Event<Player*>(EventType::PLAYER_MOVE_EVENT, &PlayerEvents::onPlayerMoveEvent));
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
