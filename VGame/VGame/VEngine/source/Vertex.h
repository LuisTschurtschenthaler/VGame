#ifndef VERTEX_H
#define VERTEX_H

#include <GLM/glm.hpp>
#include "TextureAtlas.h"


struct ChunkVertex {
	unsigned int data[2];
};

struct BasicVertex {
	glm::vec3 position;
	glm::vec2 texCoords;

	BasicVertex()
		: position(glm::vec3(0.f)), texCoords(glm::vec2(0.f)) {
	}

	BasicVertex(const glm::vec3& position, const glm::vec2& texCoords)
		: position(position), texCoords(texCoords) {
	}
};

#endif // VERTEX_H
