#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <GLM/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 uvWithTextureID;


	Vertex()
		: position(glm::vec3(0.f)), normal(glm::vec3(0.f)), uvWithTextureID(glm::vec3(0.f)) { }

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 uvWithTextureID)
		: position(position), normal(normal), uvWithTextureID(uvWithTextureID) {
	}

	Vertex(glm::vec3 position, glm::vec3 normal)
		: position(position), normal(normal) {
	}

	Vertex(glm::vec3 position)
		: position(position) { }

};

#endif // VERTEX_H