#include "Structure.h"
#include "Random.h"
#include "BlockID.h"
#include "Constants.h"
#include "ChunkManager.h"
#include "Chunk.h"


const int Structure::CROWN_RADIUS = 1;
const int Structure::BUSH_RADIUS = 2;


void Structure::build(ChunkManager& chunkManager) {
	for(auto& structureBlock : _structureBlocks) {
		//if(chunkManager.getBlockID(structureBlock.position) == BlockID::AIR)
			chunkManager.placeBlock(structureBlock.position, structureBlock.block);
	}
}

void Structure::generateTree(const LocationXYZ& pos) {
	int height = Random::get(5, 7);

	int y = pos.y + height;
	fillXZ({ pos.x - CROWN_RADIUS, y - 1, pos.z - CROWN_RADIUS },
		   { pos.x + CROWN_RADIUS, y - 1, pos.z + CROWN_RADIUS },
		   BlockID::OAK_LEAVE);

	fillXZ({ pos.x - BUSH_RADIUS, y - 2, pos.z - BUSH_RADIUS },
		   { pos.x + BUSH_RADIUS, y - 2, pos.z + BUSH_RADIUS },
		   BlockID::OAK_LEAVE);

	fillXZ({ pos.x - BUSH_RADIUS, y - 3, pos.z - BUSH_RADIUS },
		   { pos.x + BUSH_RADIUS, y - 3, pos.z + BUSH_RADIUS },
		   BlockID::OAK_LEAVE);

	fillY(pos, BlockID::OAK_LOG, height);
	clearEdges({ pos.x, y - 2, pos.z }, BUSH_RADIUS);
	clearEdges({ pos.x, y - 1, pos.z }, CROWN_RADIUS);
	_structureBlocks.emplace_back(pos.x, y, pos.z, BlockID::OAK_LEAVE);
}

void Structure::generateCactus(const LocationXYZ& pos) {
	fillY({ pos.x, pos.y, pos.z }, BlockID::CACTUS, Random::get(3, 4));
}


void Structure::fillXZ(const LocationXYZ& start, const LocationXYZ& end, BlockID block) {
	for(int x = start.x; x <= end.x; x++)
	for(int z = start.z; z <= end.z; z++)
		_structureBlocks.emplace_back(x, start.y, z, block);
}

void Structure::fillY(const LocationXYZ& start, BlockID block, int height) {
	for(int y = start.y + 1; y <= start.y + height; y++)
		_structureBlocks.emplace_back(start.x, y, start.z, block);
}

void Structure::clearEdges(const LocationXYZ& pos, int distance) {
	if(Random::get(0, 10) <= 7) _structureBlocks.emplace_back(pos.x + distance, pos.y, pos.z + distance, BlockID::AIR);
	if(Random::get(0, 10) <= 7) _structureBlocks.emplace_back(pos.x + distance, pos.y, pos.z - distance, BlockID::AIR);
	if(Random::get(0, 10) <= 7) _structureBlocks.emplace_back(pos.x - distance, pos.y, pos.z + distance, BlockID::AIR);
	if(Random::get(0, 10) <= 7) _structureBlocks.emplace_back(pos.x - distance, pos.y, pos.z - distance, BlockID::AIR);
}
