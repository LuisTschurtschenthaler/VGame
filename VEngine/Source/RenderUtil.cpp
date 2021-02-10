#include <GLEW/GL/glew.h>
#include "RenderUtil.h"


void RenderUtil::initGraphics() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_CLAMP);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderUtil::clearScreen() {
	glClearColor(5.f/255.f, 15.f/255.f, 52.f/255.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
