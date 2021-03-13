#include <GLEW/GL/glew.h>
#include "Skybox.h"
#include "Shader.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Game.h"
#include "CoreEngine.h"
#include "Timer.h"


std::vector<float> Skybox::_vertices = std::vector<float>({
	 1.f,  1.f,  1.f,
	-1.f,  1.f,  1.f,
	 1.f, -1.f,  1.f,
	 1.f,  1.f, -1.f,
	-1.f, -1.f,  1.f,
	 1.f, -1.f, -1.f,
	-1.f,  1.f, -1.f,
	-1.f, -1.f, -1.f
});

std::vector<unsigned int> Skybox::_indices = std::vector<unsigned int>({
	6, 7, 5,
	5, 3, 6,
	4, 7, 6,
	6, 1, 4,
	5, 2, 0,
	0, 3, 5,
	4, 1, 0,
	0, 2, 4,
	6, 3, 0,
	0, 1, 6,
	7, 4, 5,
	5, 4, 2
});


Skybox::Skybox() 
	: _isBuffered(false) {

	_skyboxShader = new Shader("skybox_vert.glsl", "skybox_frag.glsl");
}

Skybox::~Skybox() {
	_vertices.clear();
	_vertices.shrink_to_fit();

	_indices.clear();
	_indices.shrink_to_fit();

	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_IBO);

	delete _skyboxShader;
}


void Skybox::update() {
	_skyboxShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_skyboxShader->setMat4("view", glm::mat4(glm::mat3(World::getPlayer().camera->getView())));
	_skyboxShader->setFloat("dayTime", Game::dayTime);
}

void Skybox::draw() {
	if(!_isBuffered) {
		glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
		_isBuffered = true;
	}

	_skyboxShader->bind();
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*) 0);
	glEnableVertexAttribArray(0);

	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void*) 0);
	glDepthFunc(GL_LESS);
	
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	_skyboxShader->unbind();
}
