#include "RenderUtil.h"
#include <GLEW/GL/glew.h>


void RenderUtil::initGraphics() {
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RenderUtil::clearScreen() {
	//glClearColor(0.2f, 0.3f, 0.7f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
