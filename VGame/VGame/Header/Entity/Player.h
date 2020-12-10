#ifndef PLAYER_H
#define PLAYER_H

#include <GLM/glm.hpp>
#include "Entity.h"
#include "Camera.h"
#include "AABB.h"

class ChunkManager;


class Player : public Entity {

public:
	AABB box;
	Camera* camera;
	glm::vec3 velocity;
	bool isFlying, isOnGround, isJumping, isSwimming;

private:
	glm::vec3 _lastChunkPosition;
	bool _mouseLocked;


public:
	Player();
	~Player();

	void input(ChunkManager* chunkManager);
	void update(ChunkManager* chunkManager);
	void doCollision(ChunkManager* chunkManager);

	void setSpawnPoint(glm::vec3 spawnPoint);

private:
	void _handleKeyboardInputs(ChunkManager* chunkManager);
	void _handleMouseInputs();
	glm::vec3 _toHorizontal(const glm::vec3& vec);

};

#endif // PLAYER_H
