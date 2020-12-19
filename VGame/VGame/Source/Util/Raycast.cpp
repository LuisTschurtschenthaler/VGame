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
BlockPositionXYZ Raycast::rayCast(Camera* camera, ChunkManager* chunkManager) {
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
	glm::vec4 worldCoords = glm::inverse(camera->getView()) * eyeCoords; // <------ eyeRay.xyz should be
	glm::vec4 mouseRay = glm::normalize(worldCoords);

	glm::vec3 finalRay = glm::vec3(mouseRay.x, mouseRay.y, mouseRay.z);
	//std::cout << finalRay.x << " " << finalRay.y << " " << finalRay.z << std::endl;

	/* --------------------------------------------------------------------------------------- */

	glm::vec3 targetBlockPos(-1.f);

	for(int i = 0; i < 100; i++) {
		targetBlockPos = camera->position + static_cast<float>(i) * finalRay;
		BlockType blockType = chunkManager->getBlock({ 
			static_cast<int>(targetBlockPos.x),
			static_cast<int>(targetBlockPos.y),
			static_cast<int>(targetBlockPos.z)
		});

		if(blockType != BlockType::AIR && 
		   blockType != BlockType::WATER && 
		   blockType != BlockType::LAVA)
			return { static_cast<int>(targetBlockPos.x),
					 static_cast<int>(targetBlockPos.y),
					 static_cast<int>(targetBlockPos.z) 
			};
	}

	//std::cout << "TGP: " << targetBlockPos.x << " " << targetBlockPos.y << " " << targetBlockPos.z << std::endl;
	/* --------------------------------------------------------------------------------------- 

	int targetX = static_cast<int>(targetBlockPos.x);
	int targetY = static_cast<int>(targetBlockPos.y);
	int targetZ = static_cast<int>(targetBlockPos.z);
	
	if(targetBlockPos.x == -1.f)
		return { targetX, targetY, targetZ };

	glm::vec3 change = AABB::rayIntersectionWithBlock(camera->position, targetBlockPos, glm::vec3(targetX, targetY, targetZ));
	targetBlockPos += change;

	return {
			static_cast<int>(targetBlockPos.x),
			static_cast<int>(targetBlockPos.y),
			static_cast<int>(targetBlockPos.z)
	};*/
}
