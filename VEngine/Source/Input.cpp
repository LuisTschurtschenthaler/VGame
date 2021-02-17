#include "Input.h"
#include "Window.h"


Key::Key(int key)
	: key(key), wasPressed(false), isReleased(false) {
}

void Key::update() {
	if(!wasPressed && Input::isKeyPressed(key))
		wasPressed = true;

	if(!isReleased && wasPressed && !Input::isKeyPressed(key))
		isReleased = true;
}

bool Key::wasPressedAndReleased() {
	if(wasPressed && isReleased) {
		wasPressed = false;
		isReleased = false;
		return true;
	}
	return false;
}


unsigned int Input::_keyStates[GLFW_KEY_LAST];
unsigned int Input::_keyStatesPrevious[GLFW_KEY_LAST];
unsigned int Input::_mouseButtonStates[GLFW_MOUSE_BUTTON_LAST];
unsigned int Input::_mouseButtonStatesPrevious[GLFW_MOUSE_BUTTON_LAST];
int Input::_mouseScollState[2];

glm::vec2 Input::_mousePosition = { 0.f, 0.f };
glm::vec2 Input::_mousePositionPrevious = { 0.f, 0.f };

bool Input::_mouseVisible = false;


void Input::update() {
	for(int i = 0; i < GLFW_KEY_LAST; i++)
		_keyStatesPrevious[i] = _keyStates[i];

	for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		_mouseButtonStatesPrevious[i] = _keyStates[i];

	_mouseScollState[0] = 0;
	_mouseScollState[1] = 0;
}

bool Input::isKeyPressed(const int& key) {
	return (_keyStates[key] == GLFW_PRESS
			|| _keyStates[key] == GLFW_REPEAT);
}

bool Input::isKeyPressedRepeatedly(const int& key) {
	return false;
}

bool Input::isMouseButtonPressed(const int& button) {
	return (_mouseButtonStates[button] == GLFW_PRESS);
}

void Input::setCursorVisible(bool visible) {
	glfwSetInputMode(Window::getWindow(), GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Input::setCallbacks(GLFWwindow* window) {
	glfwSetKeyCallback(window, Input::_keyCallback);
	glfwSetMouseButtonCallback(window, Input::_mouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::_mousePositionCallback);
	glfwSetScrollCallback(window, Input::_mouseScrollCallback);
}

void Input::setMousePosition(glm::vec2 position) {
	glfwSetCursorPos(Window::getWindow(), position.x, position.y);
	_mousePosition = position;
}

void Input::_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	_keyStates[key] = action;
}

void Input::_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	_mouseButtonStates[button] = action;
}

void Input::_mousePositionCallback(GLFWwindow* window, double x, double y) {
	_mousePositionPrevious = _mousePosition;
	_mousePosition = glm::vec2(x, y);
}

void Input::_mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	_mouseScollState[0] += xOffset;
	_mouseScollState[1] += yOffset;
}
