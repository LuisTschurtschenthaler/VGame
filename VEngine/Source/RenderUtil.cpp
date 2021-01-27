#include <GLEW/GL/glew.h>
#include "RenderUtil.h"


void RenderUtil::initGraphics() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderUtil::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
