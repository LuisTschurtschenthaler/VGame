#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include <vector>


class Crosshair {

public:
	static void draw();

private:
	static void _drawLines(const float& halfWindowWidth, const float& halfWindowHeight);
	static void _drawDebugLines(const float& halfWindowWidth, const float& halfWindowHeight);

};

#endif // CROSSHAIR_H
