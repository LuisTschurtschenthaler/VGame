#ifndef PLAYER_H
#define PLAYER_H

#include <GLM/glm.hpp>
#include "Entity.h"
#include "Camera.h"
#include "AABB.h"

class World;
class ChunkManager;


class Player : public Entity {

public:
	Camera* camera;
	glm::vec3 velocity;
	bool isFlying, isOnGround, isJumping, isSwimming, isSprinting;

private:
	AABB _box;
	ChunkManager* _chunkManager;
	glm::vec3 _lastChunkPosition;
	bool _mouseLocked;
	float _jump;


public:
	Player();
	~Player();

	void setSpawnPoint(glm::vec3 spawnPoint);
	void setToWorld(World* world);
	
	void input();
	void doCollision();
	void update();

private:
	void _handleKeyboardInputs(ChunkManager* chunkManager);
	void _handleMouseMove();
	void _handleMouseButtons();
	void _handleFOV();
	glm::vec3 _toHorizontal(const glm::vec3& vec);

};

#endif // PLAYER_H
