#include "Structure.h"
#include "Random.h"
#include "BlockID.h"
#include "Constants.h"
#include "World.h"
#include "ChunkManager.h"
#include "Chunk.h"


const int Structure::CROWN_RADIUS = 1;
const int Structure::BUSH_RADIUS = 2;


void Structure::build() {
	for(auto& structureBlock : _structureBlocks) {
		if(World::getChunkManager().getBlockID(structureBlock.position) == BlockID::AIR)
			World::getChunkManager().placeBlock(structureBlock.position, structureBlock.block);
	}
}

void Structure::generateTree(const LocationXYZ& pos, const BlockID& logBlock, const BlockID& leaveBlock) {
	int height = Random::get(4, 6);

	int y = pos.y + height;
	fillY(pos, logBlock, height - 1);

	fillXZ({ pos.x - CROWN_RADIUS, y - 1, pos.z - CROWN_RADIUS },
		   { pos.x + CROWN_RADIUS, y - 1, pos.z + CROWN_RADIUS },
		   leaveBlock);

	fillXZ({ pos.x - BUSH_RADIUS, y - 2, pos.z - BUSH_RADIUS },
		   { pos.x + BUSH_RADIUS, y - 2, pos.z + BUSH_RADIUS },
		   leaveBlock);

	fillXZ({ pos.x - BUSH_RADIUS, y - 3, pos.z - BUSH_RADIUS },
		   { pos.x + BUSH_RADIUS, y - 3, pos.z + BUSH_RADIUS },
		   leaveBlock);

	clearEdges({ pos.x, y - 2, pos.z }, BUSH_RADIUS);
	clearEdges({ pos.x, y - 1, pos.z }, CROWN_RADIUS);
	_structureBlocks.emplace_back(pos.x, y, pos.z, leaveBlock);
}

void Structure::generateCactus(const LocationXYZ& pos) {
	const std::vector<LocationXYZ> nearbyBlocks = {
		{ -1, 0, 0 },
		{  1, 0, 0 },
		{ 0, 0, -1 },
		{ 0, 0,  1 }
	};

	for(auto& nearbyLocation : nearbyBlocks) {
		if(World::getChunkManager().getBlockID(pos + nearbyLocation) != BlockID::AIR)
			return;
	}
	fillY({ pos.x, pos.y, pos.z }, BlockID::CACTUS, Random::get(2, 3));
}


void Structure::fillXZ(const LocationXYZ& start, const LocationXYZ& end, BlockID block) {
	for(int x = start.x; x <= end.x; x++)
	for(int z = start.z; z <= end.z; z++)
		_structureBlocks.emplace_back(x, start.y, z, block);
}

void Structure::fillY(const LocationXYZ& start, BlockID block, int height) {
	for(int y = start.y; y <= start.y + height; y++)
		_structureBlocks.emplace_back(start.x, y, start.z, block);
}

void Structure::clearEdges(const LocationXYZ& pos, int distance) {
	auto removeBlock = [&](const LocationXYZ& location) {
		_structureBlocks.erase(
		std::remove_if(_structureBlocks.begin(), _structureBlocks.end(),
			[&](SturctureBlock it) { return (it.position == location); }
		));
	};

	if(Random::getIntInRange(0, 10) <= 4) removeBlock({ pos.x + distance, pos.y, pos.z + distance });
	if(Random::getIntInRange(0, 10) <= 4) removeBlock({ pos.x + distance, pos.y, pos.z - distance });
	if(Random::getIntInRange(0, 10) <= 4) removeBlock({ pos.x - distance, pos.y, pos.z + distance });
	if(Random::getIntInRange(0, 10) <= 4) removeBlock({ pos.x - distance, pos.y, pos.z - distance });
}
