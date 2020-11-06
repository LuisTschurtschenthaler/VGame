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
	//std::vector<BlockPositionXYZ> treePositions;
	//std::vector<BlockPositionXYZ> plantPositions;
	int max;

	ChunkMap()
		: max(WATER_LEVEL) {

		heightMap.fill(BlockType::AIR);
		biomeMap.fill(BlockType::AIR);
	}
};

#endif // CHUNK_MAP_H
