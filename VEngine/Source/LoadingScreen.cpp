#include <GLEW/GL/glew.h>
#include <GLM/glm.hpp>
#include "LoadingScreen.h"
#include "Shader.h"
#include "Texture.h"


float LoadingScreen::_vertices[] = {
	 0.5f,  0.5f, 0.f,	1.f, 0.f,
	 0.5f, -0.5f, 0.f,	1.f, 1.f,
	-0.5f, -0.5f, 0.f,	0.f, 1.f,
	-0.5f,  0.5f, 0.f,	0.f, 0.f
};

unsigned int LoadingScreen::_indices[] = {
	0, 1, 3,
	1, 2, 3
};

Shader* LoadingScreen::_shader = nullptr;
Texture* LoadingScreen::_textureLogo = nullptr;
unsigned int LoadingScreen::_VBO = 0;
unsigned int LoadingScreen::_IBO = 0;


void LoadingScreen::init() {
	_shader = new Shader("basic_vert.glsl", "basic_frag.glsl");
	_textureLogo = new Texture("Window/Logo.png", 1);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);
}

void LoadingScreen::draw(const int& loadedChunks) {
	_shader->bind();
	_textureLogo->bind();
	_shader->setInt("test", _textureLogo->getID());

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	_textureLogo->unbind();
	_shader->unbind();
}
