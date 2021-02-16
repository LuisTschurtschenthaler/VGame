#ifndef BLOCK_EVENTS
#define BLOCK_EVENTS

#include "Coordinates.h"
#include "ChunkManager.h"
#include "Raycast.h"
#include "Camera.h"
#include "World.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"


class BlockEvents {

public:
	static void onBlockBreak(ChunkManager* chunkManager) {
		LocationXYZ blockPosition = Raycast::getBlockToBreak();
		if(blockPosition.x == -1.f) return;

		BlockID blockID = World::getChunkManager().getBlockID(blockPosition);
		World::getParticleSystem().addParticleEmitter(new ParticleEmitter(blockID, blockPosition));
		chunkManager->removeBlock(blockPosition);
	}

	static void onBlockPlace(ChunkManager* chunkManager) {
		LocationXYZ blockPosition = Raycast::getBlockToPlace();

		if(blockPosition.x == -1.f) return;
		chunkManager->placeBlock(blockPosition, BlockID::TNT);
	}

};

#endif // BLOCK_EVENTS
