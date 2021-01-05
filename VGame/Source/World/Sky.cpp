#include <algorithm>
#include <GLM/glm.hpp>
#include <glm/gtx/vec_swizzle.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include "Sky.h"
#include "Shader.h"
#include "Game.h"
#include "Player.h"
#include "Camera.h"
#include "Timer.h"
#include "Window.h"
#include "WorldConstants.h"
#include "Camera.h"


Sky::Sky() {
	_shader = new Shader("sky_vert.glsl", "sky_frag.glsl");
}

Sky::~Sky() {
	vertices.clear();
	vertices.shrink_to_fit();

	indices.clear();
	indices.shrink_to_fit();

	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_IBO);
}


void Sky::init() {
	int sectorCount = 360,
		stackCount  = 360;

	float sectorStep = PI2 / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	// Vertices
	for(int i = 0; i <= stackCount; i++) {
		stackAngle = PI / 2.f - i * stackStep;
		
		float xz = 5000.f * cosf(stackAngle);
		float y  = 5000.f * sinf(stackAngle);

		for(int j = 0; j <= sectorCount; j++) {
			sectorAngle = j * sectorStep;

			float x = xz * cosf(sectorAngle);
			float z = xz * sinf(sectorAngle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	// Indices
	for(int i = 0; i < stackCount; i++) {
		unsigned int k1 = i * (sectorCount + 1);
		unsigned int k2 = k1 + sectorCount + 1;

		for(int j = 0; j < stackCount; j++, k1++, k2++) {
			if(i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if(i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Sky::draw(Player* player) {
	glm::mat4 mvp = glm::infinitePerspective(glm::radians(player->camera->fov), Window::getAspect(), NEAR_PLANE);
	mvp *= player->camera->getView();
	mvp *= glm::mat4(1.f);

	glDepthFunc(GL_LEQUAL);
	_shader->bind();

	_shader->setMat4("mvp", mvp);
	_shader->setVec3("playerPosition", player->position);
	_shader->setVec3("sunPosition", getSunPosition());
	_shader->setFloat("time", Game::dayTime);
	_shader->setBool("isUnderwater", false);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*) 0);
	glDisableVertexAttribArray(0);

	_shader->unbind();
	glDepthFunc(GL_LESS);
}

glm::vec3 Sky::getSunPosition() {
	glm::mat4 rotMatrix(1.f);
	rotMatrix = glm::rotate(rotMatrix, glm::radians(-90.f), glm::vec3(0, 1, 0));
	rotMatrix = glm::rotate(rotMatrix, glm::radians(Game::dayTime), glm::vec3(0, 0, 1));

	return (glm::xyz(rotMatrix * glm::vec4(1, 0, 0, 0)));
}


float Sky::_calculateStarIntensity(float dayTime) {
	if(dayTime >= 5 && dayTime <= 6)
		return static_cast<float>(sin((6 - dayTime) * PI / 2));

	else if(dayTime < 5 || dayTime > 19)
		return 1.f;

	else if(dayTime >= 18 && dayTime <= 19)
		return static_cast<float>(sin((dayTime - 18) * PI / 2));

	else return 0.f;
}
