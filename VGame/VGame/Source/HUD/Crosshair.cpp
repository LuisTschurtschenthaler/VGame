#include <GLEW/GL/glew.h>
#include "Crosshair.h"
#include "Texture.h"
#include "Shader.h"
#include "Window.h"


void Crosshair::draw() {
	float halfWinWidth = Window::getWidth() / 2.f;
	float halfWinHeight = Window::getHeight() / 2.f;

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glOrtho(0, Window::getWidth(), Window::getHeight(), 0.f, -1.f, 1.f);
	glMatrixMode(GL_MODELVIEW);

	glColor3ub(230, 230, 230);
	glLineWidth(3.f);

	// Horizontal line
	glBegin(GL_LINES);
	glVertex2i(halfWinWidth - 15, halfWinHeight);
	glVertex2i(halfWinWidth + 15, halfWinHeight);
	glEnd();

	// Vertical line
	glBegin(GL_LINES);
	glVertex2i(halfWinWidth, halfWinHeight + 15);
	glVertex2i(halfWinWidth, halfWinHeight - 15);
	glEnd();

	glPopMatrix();
}
