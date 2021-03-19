#ifndef PLAYER_EVENTS_H
#define PLAYER_EVENTS_H

#include <chrono>
#include "World.h"
#include "Player.h"
#include "Constants.h"
#include "ChunkManager.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"


class PlayerEvents {

public:
	static void onPlayerMoveEvent(Player* player) {
		static auto before = std::chrono::system_clock::now();
		auto now = std::chrono::system_clock::now();
		double timeDifference = std::chrono::duration<double, std::milli>(now - before).count();
		
		if(timeDifference <= 400) return;
		before = now;

		if(player->isOnGround || player->isJumping) {
			LocationXYZ standingBlock = { int(player->position.x), int(player->position.y - 2.f), int(player->position.z) };
			BlockID blockID = World::getChunkManager().getBlockID(standingBlock);

			if(blockID != BlockID::AIR) {
				LocationXYZ particlePosition = { int(player->position.x), int(player->position.y - 1.5f), int(player->position.z) };
				World::getParticleSystem().addParticleEmitter(new ParticleEmitter(blockID, particlePosition, SMALL, 10));
			}
		}
	}

};

#endif // PLAYER_EVENTS_H
