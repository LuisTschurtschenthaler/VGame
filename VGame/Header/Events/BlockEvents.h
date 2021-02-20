#ifndef BLOCK_EVENTS
#define BLOCK_EVENTS

#include "Coordinates.h"
#include "ChunkManager.h"
#include "Raycast.h"
#include "Camera.h"
#include "World.h"
#include "Constants.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"


class BlockEvents {

public:
	static void onBlockPlace() {
		LocationXYZ blockPosition = Raycast::getBlockToPlace();
		if(blockPosition.x == -1.f) return;
		
		World::getChunkManager().placeBlock(blockPosition, BlockID::TNT);
	}

	static void onBlockBreak() {
		LocationXYZ blockPosition = Raycast::getBlockToBreak();
		if(blockPosition.x == -1.f) return;

		BlockID blockID = World::getChunkManager().getBlockID(blockPosition);
		World::getParticleSystem().addParticleEmitter(new ParticleEmitter(blockID, blockPosition, MAX_BLOCK_BREAK_PARTICLES));
		World::getChunkManager().removeBlock(blockPosition);
	}

};

#endif // BLOCK_EVENTS
