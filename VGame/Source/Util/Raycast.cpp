#include <GLM/gtc/matrix_inverse.hpp>
#include "Raycast.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "Window.h"
#include "AABB.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"


// Pseudo code from: https://antongerdelan.net/opengl/raycasting.html
glm::vec3 Raycast::getDirectBlock() {
	glm::vec3 mouseRay = _getMouseRay();
	glm::vec3 targetBlockPos(-1.f);

	for(float i = 0; i < MAX_BLOCK_REACH_DISTANCE; i += 1.f) {
		targetBlockPos = World::getPlayer().camera->ENTITY->position + i * mouseRay;
		BlockID BlockID = World::getChunkManager().getBlockID({
			int(std::floor(targetBlockPos.x)),
			int(std::floor(targetBlockPos.y)),
			int(std::floor(targetBlockPos.z))
		});

		if(BlockID != BlockID::AIR &&
		   BlockID != BlockID::WATER &&
		   BlockID != BlockID::LAVA) {

			return { int(targetBlockPos.x),
					 int(targetBlockPos.y),
					 int(targetBlockPos.z)
			};
		}
	}
	return glm::vec3(-1.f);
}

LocationXYZ Raycast::getBlockToBreak() {
	glm::vec3 directBlock = getDirectBlock();
	return _getBlockPosition(directBlock);
}

LocationXYZ Raycast::getBlockToPlace() {
	glm::vec3 directBlock = getDirectBlock();

	if(directBlock == glm::vec3(-1.f))
		return _getBlockPosition(directBlock);

	int targetX = int(directBlock.x),
		targetY = int(directBlock.y),
		targetZ = int(directBlock.z);

	glm::vec3 change = AABB::rayIntersectionWithBlock(World::getPlayer().position, directBlock, glm::vec3(targetX, targetY, targetZ));
	return _getBlockPosition(directBlock += change);
}


LocationXYZ Raycast::_getBlockPosition(const glm::vec3& pos) {
	return LocationXYZ(pos.x, pos.y, pos.z);
}

glm::vec3 Raycast::_getMouseRay() {
	glm::vec2 mousePosition = Window::getMouseCenterPosition();

	// Normalised device space
	float ndsX = (2.f * mousePosition.x) / Window::getWidth() - 1.f;
	float ndsY = 1.f - (2.f * mousePosition.y) / Window::getHeight();

	// 4d homogeneous clip coordinates
	glm::vec4 clipCoords(ndsX, ndsY, -1.f, 1.f);

	// 4d eye (camera) coordinates
	glm::vec4 eyeCoords = glm::inverse(World::getPlayer().camera->getProjection()) * clipCoords;
	eyeCoords.z = -1.f;
	eyeCoords.w = 0.f;

	// 4d world coordinates
	glm::vec4 worldCoords = glm::inverse(World::getPlayer().camera->getView()) * eyeCoords;
	glm::vec4 mouseRay = glm::normalize(worldCoords);

	return glm::vec3(mouseRay.x, mouseRay.y, mouseRay.z);
}
