#ifndef VERTEX_H
#define VERTEX_H

#include <GLM/glm.hpp>
#include "TextureAtlas.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	float ambientOcclusion;

	Vertex() : position(glm::vec3(0.f)), normal(glm::vec3(0.f)), ambientOcclusion(0.f) {
	}

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, int textureID, float ao = 0.f)
		: position(position), normal(normal), ambientOcclusion(ao) {

		texCoords = TextureAtlas::getTextureCoords(uv, ((textureID == -1) ? 255 : textureID));
	}

	Vertex(glm::vec3 position, glm::vec2 texCoords)
		: position(position), texCoords(texCoords) {
	}
};

struct ParticleVertex {
	glm::vec3 position;
	glm::vec2 texCoords;

	ParticleVertex()
		: position(glm::vec3(0.f)), texCoords(glm::vec2(0.f)) {
	}
};

#endif // VERTEX_H
