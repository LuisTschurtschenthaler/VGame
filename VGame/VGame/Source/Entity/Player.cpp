#include "Player.h"
#include "Input.h"
#include "ChunkManager.h"
#include "World.h"
#include "Timer.h"


Player::Player()
	: _box({ 0.5f, 1.7f, 0.5f }), _lastChunkPosition({ 0.f }) {

	isFlying = false;
	isOnGround = false;
	isJumping = false;
	isSwimming = false;
	velocity = glm::vec3(0.f);

	_mouseLocked = false;
	camera = new Camera(this);
	_jump = 0;
}

Player::~Player() {
}


void Player::setSpawnPoint(glm::vec3 spawnPoint) {
	_lastChunkPosition = spawnPoint;
	position = spawnPoint;
}

void Player::setToWorld(World* world) {
	_chunkManager = world->getChunkManager();
}

void Player::input() {
	_handleKeyboardInputs(_chunkManager);
	_handleMouseInputs();
}

void Player::update() {

	_box.update(position);
	camera->update();

	velocity.x *= 0.85;
	if(isFlying) velocity.y *= 0.85;
	velocity.z *= 0.85;

	if((position - _lastChunkPosition).length() > 1)
		_lastChunkPosition = position;
}

void Player::doCollision() {
	position.x += velocity.x;
	_box.collision(_chunkManager, *this, { velocity.x, 0, 0 });

	position.y += velocity.y;
	_box.collision(_chunkManager, *this, { 0, velocity.y, 0 });

	position.z += velocity.z;
	_box.collision(_chunkManager, *this, { 0, 0, velocity.z });
}
