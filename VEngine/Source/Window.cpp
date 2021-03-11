#include <STB/stb_image.h>
#include "Window.h"
#include "Input.h"


GLFWwindow* Window::_window = nullptr;
GLFWmonitor* Window::_monitor = nullptr;
const GLFWvidmode* Window::_mode = nullptr;

std::string Window::_title = "";
int Window::_width = 0;
int Window::_height = 0;
bool Window::_fullscreen = false;
bool Window::_vSync = false;


void Window::create(const std::string& title, bool fullscreen, bool vSync) {
	_title = title;
	_fullscreen = fullscreen;
	_vSync = vSync;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if(!glfwInit())
		fprintf(stderr, "Error: Couldn't initialize GLFW\n");

	_monitor = glfwGetPrimaryMonitor();
	_mode = glfwGetVideoMode(_monitor);
	_width = _mode->width;
	_height = _mode->height;

	glfwWindowHint(GLFW_RED_BITS, _mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, _mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, _mode->blueBits);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	_window = glfwCreateWindow(_width, _height, _title.c_str(), fullscreen ? _monitor : nullptr, nullptr);
	if(!_window) glfwTerminate();

	glfwSetWindowAttrib(_window, GLFW_FOCUSED, GLFW_FALSE);
	glfwMakeContextCurrent(_window);

	GLenum res = glewInit();
	if(res != GLEW_OK)
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));

	glfwSetWindowSizeCallback(_window, Window::_windowResizeCallback);
	glfwSwapInterval(_vSync ? 1 : 0);

	_setWindowIcon();
	Input::setCallbacks(_window);
}

void Window::render() {
	glfwSwapBuffers(_window);
	glFlush();
}

void Window::dispose() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::handle() {
	if(Input::isKeyPressedAndReleased(GLFW_KEY_F11)) {
		_fullscreen = !_fullscreen;
		glfwSetWindowMonitor(_window, _fullscreen ? glfwGetPrimaryMonitor() : NULL, 0, 0, _width, _height, _mode->refreshRate);
		glfwSwapInterval(_vSync ? 1 : 0);
	}
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(_window);
}


void Window::_setWindowIcon() {
	GLFWimage image;
	image.pixels = stbi_load("./Resources/Textures/Window/Icon.png", &image.width, &image.height, 0, 4);
	
	glfwSetWindowIcon(_window, 1, &image);
	stbi_image_free(image.pixels);
}

void Window::_windowResizeCallback(GLFWwindow* window, int width, int height) {
	if(width <= 0 || height <= 0) return;
	
	glfwSetWindowSize(_window, width, height);
	glViewport(0, 0, width, height);
	_width = width;
	_height = height;
}
