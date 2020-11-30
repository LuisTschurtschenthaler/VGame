#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <GLM/glm.hpp>
#include "TextureAtlas.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;


	Vertex()
		: position(glm::vec3(0.f)), normal(glm::vec3(0.f)), texCoords(glm::vec2(0.f)) { }

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, int textureID)
		: position(position), normal(normal) {

		texCoords = TextureAtlas::getTextureCoords(uv, ((textureID == -1) ? 255 : textureID));
	}


	Vertex(glm::vec3 position, glm::vec3 normal)
		: position(position), normal(normal) {
	}

	Vertex(glm::vec3 position)
		: position(position) { }

};

#endif // VERTEX_H
