#include "Structure.h"
#include "Random.h"
#include "BlockType.h"
#include "WorldConstants.h"
#include "ChunkMap.h"
#include "ChunkManager.h"
#include "Chunk.h"


const int Structure::CROWN_RADIUS = 1;
const int Structure::BUSH_RADIUS = 2;


void Structure::build(ChunkManager& chunkManager) {
	for(auto& structureBlock : _structureBlocks)
		chunkManager.placeBlock(structureBlock.position, structureBlock.block);
}

void Structure::generateTree(const BlockPositionXYZ& pos) {
	int height = Random::get(5, 7);

	int y = pos.y + height;
	fillXZ({ pos.x - CROWN_RADIUS, y - 1, pos.z - CROWN_RADIUS },
		   { pos.x + CROWN_RADIUS, y - 1, pos.z + CROWN_RADIUS },
		   BlockType::OAK_LEAVE);
	clearEdges({ pos.x, y - 1, pos.z }, CROWN_RADIUS);

	fillXZ({ pos.x - BUSH_RADIUS, y - 2, pos.z - BUSH_RADIUS },
		   { pos.x + BUSH_RADIUS, y - 2, pos.z + BUSH_RADIUS },
		   BlockType::OAK_LEAVE);
	clearEdges({ pos.x, y - 2, pos.z }, BUSH_RADIUS);

	fillXZ({ pos.x - BUSH_RADIUS, y - 3, pos.z - BUSH_RADIUS },
		   { pos.x + BUSH_RADIUS, y - 3, pos.z + BUSH_RADIUS },
		   BlockType::OAK_LEAVE);

	fillY(pos, BlockType::OAK_LOG, height);
	_structureBlocks.emplace_back(pos.x, y, pos.z, BlockType::OAK_LEAVE);
}

void Structure::generateCactus(const BlockPositionXYZ& pos) {
	fillY({ pos.x, pos.y, pos.z }, BlockType::CACTUS, Random::get(3, 5));
}


void Structure::fillXZ(const BlockPositionXYZ& start, const BlockPositionXYZ& end, BlockType block) {
	for(int x = start.x; x <= end.x; x++) {
		for(int z = start.z; z <= end.z; z++)
			_structureBlocks.emplace_back(x, start.y, z, block);
	}
}

void Structure::fillY(const BlockPositionXYZ& start, BlockType block, int height) {
	for(int y = start.y + 1; y <= start.y + height; y++)
		_structureBlocks.emplace_back(start.x, y, start.z, block);
}

void Structure::clearEdges(const BlockPositionXYZ& pos, int distance) {
	if(Random::get(0, 10) <= 8) _structureBlocks.emplace_back(pos.x + distance, pos.y, pos.z + distance, BlockType::AIR);
	if(Random::get(0, 10) <= 8) _structureBlocks.emplace_back(pos.x + distance, pos.y, pos.z - distance, BlockType::AIR);
	if(Random::get(0, 10) <= 8) _structureBlocks.emplace_back(pos.x - distance, pos.y, pos.z + distance, BlockType::AIR);
	if(Random::get(0, 10) <= 8) _structureBlocks.emplace_back(pos.x - distance, pos.y, pos.z - distance, BlockType::AIR);
}
