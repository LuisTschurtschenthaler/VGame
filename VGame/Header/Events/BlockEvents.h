#ifndef BLOCK_EVENTS
#define BLOCK_EVENTS

#include "Coordinates.h"
#include "ChunkManager.h"
#include "World.h"
#include "BlockID.h"
#include "Player.h"
#include "AABB.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"


class BlockEvents {

public:
	static void onBlockPlace(LocationXYZ blockLocation, BlockID blockID) {
		if(blockLocation.x == -1.f) return;
		
		const BlockID& blockBefore = World::getChunkManager().getBlockID(blockLocation);
		World::getChunkManager().placeBlock(blockLocation, blockID);

		if(AABB::isColliding(World::getPlayer()))
			World::getChunkManager().placeBlock(blockLocation, blockBefore);
	}

	static void onBlockBreak(LocationXYZ blockLocation) {
		if(blockLocation.x == -1.f) return;

		const BlockID& blockID = World::getChunkManager().getBlockID(blockLocation);
		World::getParticleSystem().addParticleEmitter(new ParticleEmitter(blockID, blockLocation, NORMAL, MAX_BLOCK_BREAK_PARTICLES));
		World::getChunkManager().removeBlock(blockLocation);
	}

};

#endif // BLOCK_EVENTS
