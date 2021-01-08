#define no_init_all deprecated
#include <windows.h>
#include <wchar.h>
#include "Window.h"


Window::Window(int width, int height, const wchar_t* title)
	: _width(width), _height(height), _title(title) {

}

Window::~Window() {
}


void Window::init() {
	SetConsoleTitle(_title);
	_setFontSize(0, 26);
	_setWindowSize();
}


void Window::_setFontSize(const int& x, const int& y) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX currentFont = new CONSOLE_FONT_INFOEX();

	currentFont->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(console, 0, currentFont);

	currentFont->dwFontSize.X = x;
	currentFont->dwFontSize.Y = y;
	SetCurrentConsoleFontEx(console, 0, currentFont);
}

void Window::_setWindowSize() {
	HWND console = GetConsoleWindow();

	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, _height, _width, TRUE);
}
