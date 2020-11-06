#include "Player.h"
#include "Timer.h"
#include "Input.h"


Player::Player()
	: _flying(false), _cardinalPointRange(22.5), lastChunkPosition({ 0.f }) {

	camera = new Camera({ 0.f, 0.f, 0.f });
	_boundingBox = new AABB({ -0.25, -1.375, -0.25 }, { 0.25, 0.25, 0.25 });

}

Player::~Player() {
}


void Player::update() {
	camera->handleMouseInputs(MOUSE_SENSITIVITY);
	camera->handleKeyboardInputs(_flying ? FLY_SPEED : WALK_SPEED);

	if((camera->getPosition() - lastChunkPosition).length() > 1)
		lastChunkPosition = camera->getPosition();
}

void Player::updatePhysics() {

}

std::string Player::yawToCardinalPoint() {
	float yaw = camera->getYaw();
	if(_isInRange(yaw, 0) || _isInRange(yaw, 360, -360))
		return "N";

	else if(_isInRange(yaw, 315, -45))
		return "NW";

	else if(_isInRange(yaw, 270, -90))
		return "W";

	else if(_isInRange(yaw, 225, -135))
		return "SW";

	else if(_isInRange(yaw, 180, -180))
		return "S";

	else if(_isInRange(yaw, 135, -225))
		return "SE";

	else if(_isInRange(yaw, 90, -270))
		return "E";

	else if(_isInRange(yaw, 45, -315))
		return "NE";

	else return "N/A";
}

void Player::setSpawnPoint(glm::vec3 spawnPoint) {
	lastChunkPosition = spawnPoint;
	camera->setPosition(spawnPoint);
}


bool Player::_isInRange(float yaw, int numberToCheckPos, int numberToCheckNeg) {
	float minPos = numberToCheckPos - _cardinalPointRange,
		  maxPos = numberToCheckPos + _cardinalPointRange;
	float minNeg = numberToCheckNeg - _cardinalPointRange,
		  maxNeg = numberToCheckNeg + _cardinalPointRange;

	return ((yaw >= minPos) && (yaw <= maxPos) || (yaw >= minNeg) && (yaw <= maxNeg));
}
