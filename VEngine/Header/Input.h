#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <SDL2/SDL.h>
#include <GLM/glm.hpp>
#include "KeyCode.h"

#define TOTAL_KEYS 512
#define TOTAL_MOUSEBUTTONS 256


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

	static bool _isEnabled;
	static SDL_Event _event;
	static int _mouseX;
	static int _mouseY;

	static std::map<int, bool> _keyStates;
	static std::map<int, bool> _mousebuttonStates;


public:
	static void init();
	static void update();

	static bool isKeyPressed(const int& key);
	static bool isMousebuttonPressed(const int& button);

	static void setCursorVisible(bool value);

	static glm::vec2 getMousePosition() { return glm::vec2(_mouseX, _mouseY); }
	static void setMousePosition(glm::vec2 position);

};


#endif // INPUT_H