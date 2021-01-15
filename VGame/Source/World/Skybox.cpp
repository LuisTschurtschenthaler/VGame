#include <GLEW/GL/glew.h>
#include "Skybox.h"
#include "Shader.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Game.h"


float Skybox::skyboxVertices[] = {
	/* Right */
	 1.f, -1.f,  1.f,
	 1.f,  1.f,  1.f,
	 1.f, -1.f, -1.f,

	 1.f,  1.f, -1.f,
	 1.f, -1.f, -1.f,
	 1.f,  1.f,  1.f,

	 /* Left */
	 -1.f, -1.f, -1.f,
	 -1.f,  1.f, -1.f,
	 -1.f, -1.f,  1.f,

	 -1.f,  1.f,  1.f,
	 -1.f, -1.f,  1.f,
	 -1.f,  1.f, -1.f,
	
	/* Top */   
	-1.f,  1.f, -1.f,
	 1.f,  1.f, -1.f,
	-1.f,  1.f,  1.f,
	
	 1.f,  1.f,  1.f,
	-1.f,  1.f,  1.f,
	 1.f,  1.f, -1.f,
	
	 /* Bottom */  
	-1.f, -1.f, -1.f,
	-1.f, -1.f,  1.f,
	 1.f, -1.f, -1.f,
	
	 1.f, -1.f, -1.f,
	-1.f, -1.f,  1.f,
	 1.f, -1.f,  1.f,
	
	/* Front */
	-1.f,  1.f, -1.f,
	-1.f, -1.f, -1.f,
	 1.f, -1.f, -1.f,
	
	 1.f, -1.f, -1.f,
	 1.f,  1.f, -1.f,
	-1.f,  1.f, -1.f,
	
	 /* Back */  
	-1.f, -1.f,  1.f,
	-1.f,  1.f,  1.f,
	 1.f, -1.f,  1.f,
	
	 1.f,  1.f,  1.f,
	 1.f, -1.f,  1.f,
	-1.f,  1.f,  1.f
};


Skybox::Skybox() {
	_skyboxShader = new Shader("skybox_vert.glsl", "skybox_frag.glsl");

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
}

Skybox::~Skybox() {
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);

	delete _skyboxShader;
}


void Skybox::update() {
	_skyboxShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_skyboxShader->setMat4("view", glm::mat4(glm::mat3(World::getPlayer().camera->getView())));
	_skyboxShader->setFloat("dayTime", (PI / Game::dayTime));
}

void Skybox::draw() {
	_skyboxShader->bind();
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
	
	glBindVertexArray(0);
	_skyboxShader->unbind();
}
