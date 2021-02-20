#ifndef INPUT_H
#define INPUT_H

#include <chrono>
#include <vector>
#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>


class Input {

public:
	static bool mouseLocked;

private:
	static unsigned int _keyStates[GLFW_KEY_LAST];
	static unsigned int _keyStatesPrevious[GLFW_KEY_LAST];
	static std::chrono::time_point<std::chrono::system_clock> _timeStamps[GLFW_KEY_LAST];
	
	static unsigned int _mouseButtonStates[GLFW_MOUSE_BUTTON_LAST];
	static unsigned int _mouseButtonStatesPrevious[GLFW_MOUSE_BUTTON_LAST];
	static int _mouseScollState[2];

	static glm::vec2 _mousePosition, _mousePositionPrevious;


public:
	static void init();
	static void update();

	static bool isKeyPressed(const int& key);
	static bool isKeyDoublePressed(const int& key);
	static bool isKeyPressedAndReleased(const int& key);

	static bool isMouseButtonPressed(const int& button);

	static void setCursorVisible(bool value);
	static void setCallbacks(GLFWwindow* window);

	static void setMousePosition(glm::vec2 position);
	static const glm::vec2 getMousePosition() { return _mousePosition; }

	static void setMouseVisible(bool visible) { mouseLocked = visible; }
	static const bool isMouseVisible() { return mouseLocked; }

private:
	static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void _mousePositionCallback(GLFWwindow* window, double x, double y);
	static void _mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

};

#endif // INPUT_H
