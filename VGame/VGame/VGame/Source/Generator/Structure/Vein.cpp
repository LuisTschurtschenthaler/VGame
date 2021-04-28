#include "Vein.h"
#include "Chunk.h"
#include "Random.h"
#include "World.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "BlockID.h"


const std::vector<LocationXYZ> Vein::_directions = {
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
		
		LocationXYZ veinLoc;
		veinLoc.y = Random::getIntInRange(0, CHUNK_HEIGHT - 1);

		if(veinLoc.y >= veinSettings.minHeight + actualSize 
		   && veinLoc.y <= veinSettings.maxHeight - actualSize) {
			
			veinLoc.x = worldCoord.x + Random::getIntInRange(0, CHUNK_SIZE - 1);
			veinLoc.z = worldCoord.z + Random::getIntInRange(0, CHUNK_SIZE - 1);
			vein.push_back(veinLoc);

			for(int i = 0; i <= actualSize; i++) {
				int dir = Random::getIntInRange(0, _directions.size() - 1);
				int veinBlock = Random::getIntInRange(0, vein.size() - 1);

				LocationXYZ veinPos = vein[veinBlock] + _directions[dir];
				vein.push_back(veinPos);
			}

			for(auto& blockPosition : vein)
				World::getChunkManager().replaceBlock(blockPosition, BlockID::STONE, veinSettings.block);
		}
	}
}
