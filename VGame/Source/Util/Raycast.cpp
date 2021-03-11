#include <GLM/gtc/matrix_inverse.hpp>
#include "Raycast.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "Window.h"
#include "AABB.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Input.h"


RayHit Raycast::getBlockToPlace() {
	glm::vec3 position = World::getPlayer().position;
	glm::vec3 direction = World::getPlayer().camera->front;

	glm::vec3 blockPos, sign;

	for(uint8_t i = 0; i < 3; i++)
		sign[i] = direction[i] > 0;

	for(int i = 0; i < MAX_BLOCK_REACH_DISTANCE + 1; i++) {
		glm::vec3 tvec = (floor(position + sign) - position) / direction;
		float t = std::min(tvec.x, std::min(tvec.y, tvec.z));

		position += direction * (t + 0.001f);

		BlockID blockID = World::getChunkManager().getBlockID({
			int(position.x),
			int(position.y),
			int(position.z)
		});

		if(BlockManager::blocks[blockID]->hasHitbox) {
			glm::vec3 normal;
			for(int j = 0; j < 3; j++) {
				normal[j] = (t == tvec[j]);
				if(sign[j]) normal[j] = -normal[j];
			}

			position += normal;

			blockPos = floor(glm::vec3{
				position.x,
				position.y,
				position.z
			});

			return { _getBlockPosition(position), LocationXYZ(-1, -1, -1), normal };
		}
	}

	return { LocationXYZ(-1, -1, -1), LocationXYZ(-1, -1, -1), glm::vec3(0.f) };
}

RayHit Raycast::getBlockToBreak() {
	glm::vec3 directBlock = getDirectBlock();
	return { LocationXYZ(-1, -1, -1), _getBlockPosition(directBlock), glm::vec3(0.f) };
}

glm::vec3 Raycast::getDirectBlock() {
	glm::vec3 mouseRay = _getMouseRay();
	glm::vec3 targetBlockPos(-1.f);

	for(float i = 0; i < MAX_BLOCK_REACH_DISTANCE; i += 0.001f) {
		targetBlockPos = World::getPlayer().position + i * mouseRay;
		BlockID BlockID = World::getChunkManager().getBlockID({
			int(targetBlockPos.x),
			int(targetBlockPos.y),
			int(targetBlockPos.z)
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

LocationXYZ Raycast::_getBlockPosition(const glm::vec3& pos) {
	return LocationXYZ(pos.x, pos.y, pos.z);
}


glm::vec3 Raycast::_getMouseRay() {
	glm::vec2 mousePosition = Input::getMousePosition();

	// Normalized device space
	float ndsX = (2.f * mousePosition.x) / Window::getWidth() - 1.f;
	float ndsY = 1.f - (2.f * mousePosition.y) / Window::getHeight();

	// 4d homogeneous clip coordinates
	glm::vec4 clipCoords(ndsX, ndsY, -1.f, 1.f);

	// 4d eye (camera) coordinates
	glm::vec4 eyeCoords = glm::inverse(World::getPlayer().camera->getProjection()) * clipCoords;
	eyeCoords.z = -1.f;
	eyeCoords.w = 0.f;

	// 3d world coordinates
	glm::vec3 worldCoords = glm::inverse(World::getPlayer().camera->getView()) * eyeCoords;
	glm::vec3 mouseRay = glm::normalize(worldCoords);

	return mouseRay;
}
