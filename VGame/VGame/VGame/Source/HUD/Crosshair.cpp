#include <GLEW/GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Crosshair.h"
#include "Window.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Constants.h"
#include "Game.h"



void Crosshair::draw() {
	float halfWindowWidth = Window::getWidth() / 2.f,
		  halfWindowHeight = Window::getHeight() / 2.f;

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(4.f);

	if(Game::debugMode)
		_drawDebugLines(halfWindowWidth, halfWindowHeight);
	else _drawLines(halfWindowWidth, halfWindowHeight);

	glPopMatrix();
}


void Crosshair::_drawLines(const float& halfWindowWidth, const float& halfWindowHeight) {
	/* Horizontal line */
	glOrtho(0, Window::getWidth(), Window::getHeight(), 0.f, NEAR_PLANE, FAR_PLANE);
	glBegin(GL_LINES);
		glColor3f(1.f, 1.f, 1.f);
		glVertex2f(halfWindowWidth - 20, halfWindowHeight);
		glVertex2f(halfWindowWidth + 20, halfWindowHeight);
	glEnd();
	
	/* Vertical line */
	glBegin(GL_LINES);
		glColor3f(1.f, 1.f, 1.f);
		glVertex2f(halfWindowWidth, halfWindowHeight + 20);
		glVertex2f(halfWindowWidth, halfWindowHeight - 20);
	glEnd();
}

void Crosshair::_drawDebugLines(const float& halfWindowWidth, const float& halfWindowHeight) {
	glOrtho(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, NEAR_PLANE, FAR_PLANE);

	glRotatef(World::getPlayer().pitch, 1, 0, 0);
	glRotatef(-World::getPlayer().yaw, 0, 1, 0);
	glScalef(40.f, 40.f, 40.f);

	/* X-axis (red) */
	glBegin(GL_LINES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(1.f, 0.f, 0.f);
	glEnd();

	/* Z-axis (blue) */
	glBegin(GL_LINES);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, 1.f);
	glEnd();

	/* Y-axis (green) */
	glBegin(GL_LINES);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 1.f, 0.f);
	glEnd();
}
