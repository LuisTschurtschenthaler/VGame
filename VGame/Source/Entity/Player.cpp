#include "Player.h"
#include "Input.h"
#include "ChunkManager.h"
#include "World.h"
#include "Timer.h"


Player::Player()
	: Entity() {
	
	World::getChunkManager().findSpawnPoint(*this);
	box = AABB({ 0.32f, 1.75f, 0.32f });
	camera = new Camera(this);

	isFlying = false;
	isOnGround = false;
	isJumping = false;
	isSwimming = false;
	isSprinting = false;
	velocity = glm::vec3(0.f);

	_mouseTimer = new Timer();
	_selectedBlock = BlockID::PINK_GLASS;
	_jump = 0;
}

Player::~Player() {
	delete _mouseTimer;
	delete camera;
}


void Player::update() {
	//isSwimming = (World::getChunkManager().getBlockID({ int(position.x), int(position.y - 1), int(position.z) }) == WATER);
	//isUnderwater = (World::getChunkManager().getBlockID({ int(position.x), int(position.y), int(position.z) }) == WATER);
	isOnGround = BlockManager::getBlock(World::getChunkManager().getBlockID({ int(position.x), int(position.y - 1.8f), int(position.z) })).hasHitbox;

 	_input();
	camera->update();
	box.update(position);

	float drag = isFlying ? 0.925f : 0.85f;
	if(isFlying) velocity.y *= 0.925f;
	velocity.x *= drag;
	velocity.z *= drag;


	auto doCollision = [&](const glm::vec3& velocity) {
		position += velocity;
		AABB::handleCollision(*this, velocity);
	};

	doCollision({ velocity.x, 0, 0 });
	doCollision({ 0, velocity.y, 0 });
	doCollision({ 0, 0, velocity.z });
}
