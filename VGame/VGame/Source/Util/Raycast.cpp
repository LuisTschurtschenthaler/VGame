

// Pseudo code from: https://antongerdelan.net/opengl/raycasting.html
glm::vec3 Raycast::rayCast(Camera* camera, ChunkManager* chunkManager, float distance) {
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

	
	for(float x = startPosition.x; x < endPosition.x; x += distance)
	for(float y = startPosition.y; x < endPosition.y; y += distance)
	for(float z = startPosition.x; z < endPosition.x; z += distance) {

		Block* block = BlockUtil::blocks[chunkManager->getBlock({ x, y, z })];
		if(block->hasHitbox)
			return { x, y, z };
	}
	
	return { 0, 0, 0 };
}
