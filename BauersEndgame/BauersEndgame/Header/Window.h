#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class Window {

private:
	int _width, _height;
	const wchar_t* _title;


public:
	Window(int width, int height, const wchar_t* title);
	~Window();

	void init();

private:
	void _setFontSize(const int& x, const int& y);
	void _setWindowSize();


};

#endif // WINDOW_H
