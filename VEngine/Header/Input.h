#ifndef INPUT_H
#define INPUT_H

#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include "KeyCode.h"


struct Key {
	int key;
	bool wasPressed, isReleased;

	Key(int key);

	void update();
	bool wasPressedAndReleased();
};


class Input {

private:
	static Key fullscreenKey;

	static unsigned int _keyStates[GLFW_KEY_LAST];
	static unsigned int _keyStatesPrevious[GLFW_KEY_LAST];
	static unsigned int _mouseButtonStates[GLFW_MOUSE_BUTTON_LAST];
	static unsigned int _mouseButtonStatesPrevious[GLFW_MOUSE_BUTTON_LAST];
	static int _mouseScollState[2];

	static glm::vec2 _mousePosition, _mousePositionPrevious;
	static bool _mouseVisible;

public:
	static void update();

	static bool isKeyPressed(const int& key);
	static bool isKeyPressedRepeatedly(const int& key);
	static bool isMouseButtonPressed(const int& button);

	static void setCursorVisible(bool value);
	static void setCallbacks(GLFWwindow* window);

	static void setMousePosition(glm::vec2 position);
	static const glm::vec2 getMousePosition() { return _mousePosition; }

	static void setMouseVisible(bool visible) { _mouseVisible = visible; }
	static const bool isMouseVisible() { return _mouseVisible; }

private:
	static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void _mousePositionCallback(GLFWwindow* window, double x, double y);
	static void _mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

};

#endif // INPUT_H
