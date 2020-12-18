#include "Input.h"
#include "Window.h"
#include "SDLHandler.h"


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


Key Input::fullscreenKey = Key(KeyCode::KEY_F11);
bool Input::_isEnabled = false;
SDL_Event Input::_event;
int Input::_mouseX = 0;
int Input::_mouseY = 0;
std::map<int, bool> Input::_keys;
std::map<int, bool> Input::_mousebuttons;


void Input::init() {
	for(int i = 0; i < TOTAL_KEYS; i++)
		_keys[i] = false;

	for(int i = 0; i < TOTAL_MOUSEBUTTONS; i++)
		_mousebuttons[i] = false;
}

void Input::update() {
	if(fullscreenKey.wasPressedAndReleased())
		Window::setFullscreen();

	while(SDL_PollEvent(&_event)) {
		// Quit
		if(_event.type == SDL_QUIT)
			setCloseRequested(true);
	
		// Window
		if(_event.type == SDL_WINDOWEVENT) {
			if(_event.window.event == SDL_WINDOWEVENT_RESIZED)
				Window::setWindowSize(_event.window.data1, _event.window.data2);
		} 

		// Keyboard
		if(_event.type == SDL_KEYDOWN)
			_keys[_event.key.keysym.scancode] = true;

		if(_event.type == SDL_KEYUP)
			_keys[_event.key.keysym.scancode] = false;

		// Mouse
		if(_event.type == SDL_MOUSEMOTION) {
			_mouseX = _event.motion.x;
			_mouseY = _event.motion.y;
		}

		if(_event.type == SDL_MOUSEBUTTONDOWN)
			_mousebuttons[_event.button.button] = true;

		if(_event.type == SDL_MOUSEBUTTONUP)
			_mousebuttons[_event.button.button] = false;

		// --------> SDL_MOUSEWHEEL
	}

	fullscreenKey.update();
}

bool Input::isKeyPressed(int key) {
	bool result = false;

	if(_isEnabled) {
		std::map<int, bool>::iterator it = _keys.find(key);
		if(it != _keys.end())
			result = _keys[key];
	}

	return result;
}

bool Input::isMousebuttonPressed(int button) {
	bool result = false;

	std::map<int, bool>::iterator it = _mousebuttons.find(button);
	if(it != _mousebuttons.end())
		result = _mousebuttons[button];

	return result;
}

void Input::setCursorVisible(bool visible) {
	visible ? SDL_ShowCursor(1) : SDL_ShowCursor(0);
	_isEnabled = !visible;
}

void Input::setMousePosition(glm::vec2 position) {
	SDLSetMousePosition(static_cast<int>(position.x), static_cast<int>(position.y));
}
