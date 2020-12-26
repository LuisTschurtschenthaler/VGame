#ifndef BLOCK_EVENTS
#define BLOCK_EVENTS

#include <iostream>


class BlockEvents {

public:
	static void onBlockBreak(int a) {
		std::cout << "BlockBreakEvent" << std::endl;
	}

	static void onBlockPlace(int a) {
		std::cout << "BlockPlaceEvent" << std::endl;
	}

};

#endif // BLOCK_EVENTS
