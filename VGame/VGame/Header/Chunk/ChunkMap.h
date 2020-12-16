#ifndef CHUNK_MAP_H
#define CHUNK_MAP_H

#include <vector>
#include "Coordinates.h"
#include "WorldConstants.h"
#include "Array2D.h"
#include "BlockType.h"


class ChunkMap {

public:
	Array2D<int, CHUNK_SIZE> heightMap;
	Array2D<int, CHUNK_SIZE> biomeMap;
	//Array2D<unsigned char, CHUNK_SIZE> lightMap;
	int max;

	ChunkMap()
		: max(WATER_LEVEL) {

		heightMap.fill(0);
		biomeMap.fill(0);
		//lightMap.fill(MIN_LIGHT_LEVEL);
	}
};

#endif // CHUNK_MAP_H
