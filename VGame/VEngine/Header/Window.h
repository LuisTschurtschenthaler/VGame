#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <GLM/glm.hpp>

class Window {

private:
	static int _width;
	static int _height;
	static bool _fullscreen;
	static std::string _title;


public:
	static void create(int width, int height, bool fullscreen, const std::string& title);
	static void render();
	static void dispose();

	static bool shouldClose();

	static void setFullscreen();
	static void setWindowHeight(int width, int height);

	static const int getWidth() { return _width; }
	static const int getHeight() { return _height; }
	static float getAspect() { return (static_cast<float>(_width) / static_cast<float>(_height));}
	static const glm::vec2 getWindowResolution() { return glm::vec2(static_cast<float>(_width), static_cast<float>(_height)); }
	static const glm::vec2 getMouseCenterPosition() { return glm::vec2(static_cast<float>(_width / 2.f), static_cast<float>(_height / 2.f)); }
};

#endif // WINDOW_H
