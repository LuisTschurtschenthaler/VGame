#ifndef CROSSHAIR_H
#define CROSSHAIR_H

class Texture;

class Crosshair {

private:
	static Texture* _crosshair;


public:
	static void init();
	static void draw();

};

#endif // CROSSHAIR_H
