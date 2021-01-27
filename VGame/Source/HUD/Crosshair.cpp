#include <GLEW/GL/glew.h>
#include "Crosshair.h"
#include "Window.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Constants.h"
#include "Game.h"


void Crosshair::draw() {
	float halfWinWidth = Window::getWidth() / 2.f;
	float halfWinHeight = Window::getHeight() / 2.f;

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(Game::debugMode) {
		glOrtho(-halfWinWidth, halfWinWidth, -halfWinHeight, halfWinHeight, NEAR_PLANE, FAR_PLANE);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(World::getPlayer().pitch, 1, 0, 0);
		glRotatef(-World::getPlayer().yaw, 0, 1, 0);

		glLineWidth(3.f);
		glScalef(40.f, 40.f, 40.f);

		// Y-Axis (green)
		glBegin(GL_LINES);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(0.f, 1.f, 0.f);
		glEnd();

		// X-Axis (red)
		glBegin(GL_LINES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(1.f, 0.f, 0.f);
		glEnd();

		// Z-Axis (blue)
		glBegin(GL_LINES);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(0.f, 0.f, 1.f);
		glEnd();
	}

	else {
		glOrtho(0, Window::getWidth(), Window::getHeight(), 0.f, -1.f, 1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glLineWidth(3.f);
		glColor3f(1.f, 1.f, 1.f);

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
}
