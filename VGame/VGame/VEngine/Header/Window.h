#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>


class Window {

private:
	static GLFWwindow* _window;
	static GLFWmonitor* _monitor;
	static const GLFWvidmode* _mode;

	static std::string _title;
	static int _width, _height;
	static bool _fullscreen, _vSync;


public:
	static void create(const std::string& title, bool fullscreen, bool vSync);
	static void render();
	static void dispose();

	static void handle();
	static bool shouldClose();

	static GLFWwindow* getWindow() { return _window; }
	static const int getWidth() { return _width; }
	static const int getHeight() { return _height; }

	static const float getAspect() { return (static_cast<float>(_width) / static_cast<float>(_height)); }
	static const glm::vec2 getWindowResolution() { return glm::vec2(_width, _height); }
	static const glm::vec2 getMouseCenterPosition() { return glm::vec2(_width / 2.f, _height / 2.f); }

private:
	static void _setWindowIcon();
	static void _windowResizeCallback(GLFWwindow* window, int width, int height);

};

#endif // WINDOW_H
