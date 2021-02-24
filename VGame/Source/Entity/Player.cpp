#include "Player.h"
#include "Input.h"
#include "ChunkManager.h"
#include "World.h"
#include "Timer.h"


Player::Player()
	: Entity() {
	
	World::getChunkManager().findSpawnPoint(position);
	box = AABB({ 0.3f, 1.75f, 0.3f });
	camera = new Camera(this);

	isFlying = false;
	isOnGround = false;
	isJumping = false;
	isSwimming = false;
	isSprinting = false;
	velocity = glm::vec3(0.f);

	_mouseTimer = new Timer();
	_selectedBlock = BlockID::CRAVED_PUMPKIN;
	_jump = 0;
}

Player::~Player() {
	delete camera;
	delete _mouseTimer;
}


void Player::update() {
	isSwimming = (World::getChunkManager().getChunkBlock({ int(position.x), int(position.y - 1), int(position.z) }).blockID == WATER);
	isUnderwater = (World::getChunkManager().getChunkBlock({ int(position.x), int(position.y), int(position.z) }).blockID == WATER);
	isOnGround = BlockManager::blocks[World::getChunkManager().getChunkBlock({ int(position.x), int(position.y - 1.8f), int(position.z) }).blockID]->hasHitbox;

	_input();
	camera->update();
	box.update(position);

	float drag = isFlying ? 0.95f : 0.85f;
	velocity.x *= drag;
	velocity.z *= drag;
	if(isFlying) velocity.y *= 0.9;


	auto doCollision = [&](const glm::vec3& velocity) {
		position += velocity;
		box.collision(*this, velocity);
	};

	doCollision({ velocity.x, 0, 0 });
	doCollision({ 0, velocity.y, 0 });
	doCollision({ 0, 0, velocity.z });
}
