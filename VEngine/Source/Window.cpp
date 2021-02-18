#include <STB/stb_image.h>
#include "Window.h"
#include "Input.h"


GLFWwindow* Window::_window = nullptr;

int Window::_width = 0;
int Window::_height = 0;
bool Window::_fullscreen = false;
std::string Window::_title = "";


void Window::create(int width, int height, bool fullscreen, bool vSync, const std::string& title) {
	_width = width;
	_height = height;
	_fullscreen = fullscreen;
	_title = title;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if(!glfwInit())
		fprintf(stderr, "Error: Couldn't initialize GLFW\n");

	_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
	if(!_window) glfwTerminate();
	
	glfwSetWindowAttrib(_window, GLFW_FOCUSED, GLFW_TRUE);
	glfwMakeContextCurrent(_window);

	GLenum res = glewInit();
	if(res != GLEW_OK)
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));

	glfwSetWindowSizeCallback(_window, Window::_windowResizeCallback);
	glfwSwapInterval(vSync ? 1 : 0);

	_setWindowIcon();
	Input::setCallbacks(_window);
}

void Window::render() {
	glfwSwapBuffers(_window);
	glfwPollEvents();
	glFlush();
}

void Window::dispose() {
	glfwTerminate();
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(_window);
}

void Window::setFullscreen() {
	_fullscreen = !_fullscreen;
}


void Window::_setWindowIcon() {
	GLFWimage images[2];
	images[0].pixels = stbi_load("./Resources/Textures/Window/Icon.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("./Resources/Textures/Window/Icon_low.png", &images[1].width, &images[1].height, 0, 4);
	glfwSetWindowIcon(_window, 1, images);

	stbi_image_free(images[0].pixels);
	stbi_image_free(images[1].pixels);
}

void Window::_windowResizeCallback(GLFWwindow* window, int width, int height) {
	glfwSetWindowSize(_window, width, height);
	glViewport(0, 0, width, height);
	_width = width;
	_height = height;
}
