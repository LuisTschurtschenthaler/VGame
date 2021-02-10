#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <vector>
#include "Vertex.h"

class Shader;
class Texture;


class SplashScreen {

private:
	static Shader* _shader;
	static Texture* _textureLogo;
	static float _vertices[];
	static unsigned int _indices[];
	static unsigned int _VBO, _IBO;

public:
	static void init();
	static void draw(const int& loadedChunks);

};

#endif // SPLASH_SCREEN_H
