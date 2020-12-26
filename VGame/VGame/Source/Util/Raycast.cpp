#include <vector>
#include <GLM/gtc/matrix_inverse.hpp>
#include "Raycast.h"
#include "Camera.h"
#include "ChunkManager.h"
#include "WorldConstants.h"
#include "Window.h"
#include "Block.h"
#include "AABB.h"


// Pseudo code from: https://antongerdelan.net/opengl/raycasting.html
glm::vec3 Raycast::getBlockToBreak(Camera* camera, ChunkManager* chunkManager) {
	glm::vec3 mouseRay = _getMouseRay(camera, chunkManager);
	glm::vec3 targetBlockPos(-1.f);

	for(int i = 0; i < MAX_BLOCK_REACH_DISTANCE; i++) {
		targetBlockPos = camera->position + static_cast<float>(i) * mouseRay;
		BlockType blockType = chunkManager->getBlockType({
			int(targetBlockPos.x),
			int(targetBlockPos.y),
			int(targetBlockPos.z)
		});

		if(blockType != BlockType::AIR &&
		   blockType != BlockType::WATER &&
		   blockType != BlockType::LAVA) {

			return { int(targetBlockPos.x),
					 int(targetBlockPos.y),
					 int(targetBlockPos.z) };
		}
	}
	return glm::vec3(-1.f);
}

glm::vec3 Raycast::getBlockToPlace(Camera* camera, ChunkManager* chunkManager) {
	glm::vec3 mouseRay = _getMouseRay(camera, chunkManager);
	glm::vec3 targetBlockPos(-1.f);

	for(int i = 0; i < MAX_BLOCK_REACH_DISTANCE; i++) {
		targetBlockPos = camera->position + static_cast<float>(i) * mouseRay;
		BlockType blockType = chunkManager->getBlockType({
			int(std::floor(targetBlockPos.x)),
			int(std::floor(targetBlockPos.y)),
			int(std::floor(targetBlockPos.z))
		});

		if(blockType != BlockType::AIR &&
		   blockType != BlockType::WATER &&
		   blockType != BlockType::LAVA) {

			break;
		}
	}

	if(targetBlockPos.x == -1)
		return targetBlockPos;

	int targetX = static_cast<int>(targetBlockPos.x);
	int targetY = static_cast<int>(targetBlockPos.y);
	int targetZ = static_cast<int>(targetBlockPos.z);

	glm::vec3 change = AABB::rayIntersectionWithBlock(camera->position, targetBlockPos, glm::vec3(targetX, targetY, targetZ));
	targetBlockPos += change;
	return targetBlockPos;
}

BlockPositionXYZ Raycast::getBlockPosition(const glm::vec3 pos) {
	return BlockPositionXYZ(pos.x, pos.y, pos.z);
}


glm::vec3 Raycast::_getMouseRay(Camera* camera, ChunkManager* chunkManager) {
	glm::vec2 mousePosition = Window::getMouseCenterPosition();

	// Normalised device space
	float ndsX = (2.f * mousePosition.x) / Window::getWidth() - 1.f;
	float ndsY = 1.f - (2.f * mousePosition.y) / Window::getHeight();
	glm::vec2 ndsRay(ndsX, ndsY);

	// 4d homogeneous clip coordinates
	glm::vec4 clipCoords(ndsRay, -1.f, 1.f);

	// 4d eye (camera) coordinates
	glm::vec4 eyeCoords = glm::inverse(camera->getProjection()) * clipCoords;
	eyeCoords.z = -1.f;
	eyeCoords.w = 0.f;

	// 4d world coordinates
	glm::vec4 worldCoords = glm::inverse(camera->getView()) * eyeCoords;
	glm::vec4 mouseRay = glm::normalize(worldCoords);

	return glm::vec3(mouseRay.x, mouseRay.y, mouseRay.z);
}
