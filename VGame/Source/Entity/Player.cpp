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
	_mouseLocked = false;
	_jump = 0;
}

Player::~Player() {
	delete camera;
	delete _mouseTimer;
}


void Player::update() {
	_input();

	velocity.x *= 0.85;
	if(isFlying) velocity.y *= 0.85;
	velocity.z *= 0.85;

	camera->update();
	box.update(position);

	position.x += velocity.x;
	box.collision(*this, { velocity.x, 0, 0 });

	position.y += velocity.y;
	box.collision(*this, { 0, velocity.y, 0 });

	position.z += velocity.z;
	box.collision(*this, { 0, 0, velocity.z });
}
