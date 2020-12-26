#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

#define BIT(x) (1 << x)

enum class EventCategory {
	NONE			= 0,
	GAME_EVENT		= BIT(0),
	BLOCK_EVENT		= BIT(1),
	ENTITY_EVENT	= BIT(2),
	PLAYER_EVENT	= BIT(3),
	WORLD_EVENT		= BIT(4)
};

enum EventType {
	NONE = 0,
	BLOCK_BREAK_EVENT, BLOCK_PLACE_EVENT
};


#endif // EVENT_TYPES_H
