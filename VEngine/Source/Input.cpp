#include "Input.h"
#include "Window.h"


bool Input::enabled = false;
bool Input::mouseLocked = false;

unsigned int Input::_keyStates[GLFW_KEY_LAST];
unsigned int Input::_keyStatesPrevious[GLFW_KEY_LAST];
std::chrono::time_point<std::chrono::system_clock> Input::_timeStamps[GLFW_KEY_LAST];

unsigned int Input::_mouseButtonStates[GLFW_MOUSE_BUTTON_LAST];
unsigned int Input::_mouseButtonStatesPrevious[GLFW_MOUSE_BUTTON_LAST];
int Input::_mouseScollState[2];

glm::vec2 Input::_mousePosition = { 0.f, 0.f };
glm::vec2 Input::_mousePositionPrevious = { 0.f, 0.f };


void Input::init() {
	auto now = std::chrono::system_clock::now();
	for(int i = 0; i < GLFW_KEY_LAST; i++)
		_timeStamps[i] = now;
}

void Input::update() {
	for(int i = 0; i < GLFW_KEY_LAST; i++)
		_keyStatesPrevious[i] = _keyStates[i];

	for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		_mouseButtonStatesPrevious[i] = _keyStates[i];

	_mouseScollState[0] = 0;
	_mouseScollState[1] = 0;
	glfwPollEvents();
}

bool Input::isKeyPressed(const int& key) {
	if(!enabled) return false;
	return (_keyStates[key] == GLFW_PRESS || _keyStates[key] == GLFW_REPEAT || _keyStates[key] == GLFW_DOUBLE_PRESS);
}

bool Input::isKeyDoublePressed(const int& key) {
	if(!enabled) return false;
	return ((_keyStates[key] == GLFW_DOUBLE_PRESS ) && _keyStatesPrevious[key] == GLFW_RELEASE);
}

bool Input::isKeyPressedAndReleased(const int& key) {
	if(!enabled) return false;
	return ((_keyStates[key] == GLFW_PRESS || _keyStates[key] == GLFW_REPEAT) && _keyStatesPrevious[key] == GLFW_RELEASE);
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
	if(key == GLFW_KEY_UNKNOWN) return;

	if(_keyStatesPrevious[key] == GLFW_RELEASE) {
		auto now = std::chrono::system_clock::now();
		double timeDifference = std::chrono::duration<double, std::milli>(now - _timeStamps[key]).count();
		_timeStamps[key] = now;

		if(timeDifference > 10 && timeDifference < 300)
			action = GLFW_DOUBLE_PRESS;
	}

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
