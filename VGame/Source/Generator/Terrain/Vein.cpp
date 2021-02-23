#include "Vein.h"
#include "Chunk.h"
#include "Random.h"
#include "World.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "BlockID.h"


std::vector<LocationXYZ> Vein::_directions = {
	{  1, 0, 0 },
	{ -1, 0, 0 },
	{ 0,  1, 0 },
	{ 0, -1, 0 },
	{ 0, 0,  1 },
	{ 0, 0, -1 }
};


void Vein::generate(const ChunkXZ& worldCoord, const VeinSettings& veinSettings) {
	for(int i = 0; i < veinSettings.spawnTries; i++) {
		std::vector<LocationXYZ> vein = std::vector<LocationXYZ>();
		int actualSize = Random::getIntInRange(veinSettings.minSize, veinSettings.maxSize);
		LocationXYZ veinLocation;

		veinLocation.y = Random::getIntInRange(0, CHUNK_HEIGHT - 1);

		if(veinLocation.y >= veinSettings.minHeight + actualSize 
		   && veinLocation.y <= veinSettings.maxHeight - actualSize) {
			
			veinLocation.x = worldCoord.x + Random::getIntInRange(0, CHUNK_SIZE - 1);
			veinLocation.z = worldCoord.z + Random::getIntInRange(0, CHUNK_SIZE - 1);
			vein.push_back(veinLocation);
			
			if(vein.size() <= 0)
				return;

			for(int i = 0; i <= actualSize; i++) {
				int dir = Random::getIntInRange(0, _directions.size() - 1);
				LocationXYZ veinPos = vein[Random::getIntInRange(0, vein.size() - 1)] + _directions[dir];
				vein.push_back(veinPos);
			}

			for(auto& l : vein)
				World::getChunkManager().replaceBlock(l, BlockID::STONE, veinSettings.block);
		}
	}
}
