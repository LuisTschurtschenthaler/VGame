#ifndef VERTEX_H
#define VERTEX_H

#include <GLM/glm.hpp>
#include "TextureAtlas.h"


struct Vertex {
	glm::vec3 position, normal;
	glm::vec2 texCoords;
	float ambientOcclusion;

	Vertex() 
		: position(glm::vec3(0.f)), normal(glm::vec3(0.f)), ambientOcclusion(0.f) {
	}

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv, const int& textureID, const float& ao = 0.f)
		: position(position), normal(normal), ambientOcclusion(ao) {

		texCoords = TextureAtlas::getTextureCoords(uv, ((textureID == -1) ? 255 : textureID));
	}

	Vertex(const glm::vec3& position, const glm::vec2& texCoords)
		: position(position), texCoords(texCoords) {
	}
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
