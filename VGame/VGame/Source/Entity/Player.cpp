#include "Player.h"
#include "Input.h"
#include "ChunkManager.h"
#include "Timer.h"
#include "AABB.h"
#include "FramerateCounter.h"


Player::Player() 
	: box({ 0.5f, 1.5f, 0.5f }), _lastChunkPosition({ 0.f }) {

	isFlying = false;
	isOnGround = false;
	isJumping = false;
	isSwimming = false;
	velocity = glm::vec3(0.f);

	_mouseLocked = false;
	camera = new Camera(this);
}

Player::~Player() {
}


void Player::input(ChunkManager* chunkManager) {
	_handleKeyboardInputs(chunkManager);
	_handleMouseInputs();
}

void Player::doCollision(ChunkManager* chunkManager) {
	float gravity = GRAVITY / 60;

	if(!isFlying) {
		if(!isOnGround && !isSwimming)
			velocity.y -= gravity;

		if(isSwimming)
			velocity.y -= gravity;

		isOnGround = false;
	}

	position.x += velocity.x;
	box.collision(chunkManager, *this, { velocity.x, 0, 0 });

	position.y += velocity.y;
	box.collision(chunkManager, *this, { 0, velocity.y, 0 });

	position.z += velocity.z;
	box.collision(chunkManager, *this, { 0, 0, velocity.z });
}


void Player::update(ChunkManager* chunkManager) {
	box.update(position);
	camera->update();

	velocity.x *= 0.85;
	velocity.z *= 0.85;

	if(isFlying)
		velocity.y *= 0.85;
	
	if((position - _lastChunkPosition).length() > 1)
		_lastChunkPosition = position;
}

void Player::setSpawnPoint(glm::vec3 spawnPoint) {
	_lastChunkPosition = spawnPoint;
	position = spawnPoint;
}
