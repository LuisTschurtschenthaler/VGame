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
	
	//glfwSetWindowIcon(_window, 1, load_icon);
	glfwSetWindowAttrib(_window, GLFW_FOCUSED, GLFW_TRUE);
	glfwMakeContextCurrent(_window);
	Input::setCallbacks(_window);

	GLenum res = glewInit();
	if(res != GLEW_OK)
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));

	glfwSetWindowSizeCallback(_window, Window::_windowResizeCallback);
	glfwSwapInterval(vSync ? 1 : 0);
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


void Window::_windowResizeCallback(GLFWwindow* window, int width, int height) {
	glfwSetWindowSize(_window, width, height);
	glViewport(0, 0, width, height);
	_width = width;
	_height = height;
}
