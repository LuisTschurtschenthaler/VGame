#ifndef VERTEX_H
#define VERTEX_H

#include <GLM/glm.hpp>
#include "TextureAtlas.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	float ambientOcclusion;

	Vertex() : position(glm::vec3(0.f)), normal(glm::vec3(0.f)), ambientOcclusion(3.f) { }

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, int textureID, float ao = 3.f)
		: position(position), normal(normal), ambientOcclusion(ao) {

		texCoord = TextureAtlas::getTextureCoords(uv, ((textureID == -1) ? 255 : textureID));
	}
};

#endif // VERTEX_H
