#ifndef RAYCAST_H
#define RAYCAST_H

#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_inverse.hpp>
#include "Camera.h"
#include "ChunkManager.h"
#include "WorldConstants.h"
#include "Window.h"
#include "Block.h"


class Raycast {

public:
	static glm::vec3 rayCast(Camera* camera, ChunkManager* chunkManager, float distance) {
		/*
		glm::vec2 mousePosition = Window::getMouseCenterPosition();

		// Normalised device space
		float ndsX = (2.f * mousePosition.x) / Window::getWidth() - 1.f;
		float ndsY = 1.f - (2.f * mousePosition.y) / Window::getHeight();
		glm::vec2 ndsRay(ndsX, ndsY);

		// 4d homogeneous clip coordinates
		glm::vec4 rayClip(ndsRay, -1.f, 1.f);

		// 4d eye (camera) coordinates
		glm::vec4 eyeRay = glm::inverse(camera->getProjection()) * rayClip;

		// 4d world coordinates
		glm::vec3 worldRay = glm::inverse(camera->getView()) * eyeRay; // <------ eyeRay.xyz should be
		worldRay = glm::normalize(worldRay);

		glm::vec3 startPosition = camera->position,
			endPosition = camera->position + (worldRay * distance);

		std::cout << startPosition.x << " " << startPosition.y << " " << startPosition.z<< std::endl;
		std::cout << endPosition.x << " " << endPosition.y << " " << endPosition.z<< std::endl;
		std::cout <<  std::endl;

		for(float x = startPosition.x; x < endPosition.x; x += BLOCK_SIZE)
			for(float y = startPosition.y; x < endPosition.y; y += BLOCK_SIZE)
				for(float z = startPosition.x; z < endPosition.x; z += BLOCK_SIZE) {

					BlockType block = chunkManager->getBlock({
						static_cast<int>(std::floor(x)),
						static_cast<int>(std::floor(y)),
						static_cast<int>(std::floor(z)),
					});
					if(block != AIR)
						return { x, y, z };
				}

		return { 0, 0, 0 };
	}
	*/
};

#endif // RAYCAST_H
