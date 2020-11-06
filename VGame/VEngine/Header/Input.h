#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <GLM/glm.hpp>
#include "KeyCode.h"

#define TOTAL_KEYS 512
#define TOTAL_MOUSEBUTTONS 256


class Input {

private:
	static bool _isEnabled;
	static SDL_Event _event;
	static int _mouseX;
	static int _mouseY;

	static std::map<int, bool> _keys;
	static std::map<int, bool> _mousebuttons;


public:
	static void setup();
	static void update();

	static bool isKeyPressed(int key);
	static bool isMousebuttonPressed(int button);

	static void setCursorVisible(bool value);

	static glm::vec2 getMousePosition() { return glm::vec2(_mouseX, _mouseY); }
	static void setMousePosition(glm::vec2 position);

};


struct Key {
	int key;
	bool wasPressed, isReleased;

	Key(int key)
		: key(key), wasPressed(false), isReleased(false) {
	}

	void update() {
		if (!wasPressed && Input::isKeyPressed(key))
			wasPressed = true;

		if (!isReleased && wasPressed && !Input::isKeyPressed(key))
			isReleased = true;
	}

	bool wasPressedAndReleased() {
		if (wasPressed && isReleased) {
			wasPressed = false;
			isReleased = false;
			return true;
		}
		return false;
	}

};


#endif // INPUT_H