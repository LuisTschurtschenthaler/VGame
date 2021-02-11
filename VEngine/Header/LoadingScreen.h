#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <vector>
#include "Vertex.h"

class Shader;
class Texture;


class LoadingScreen {

private:
	static Shader* _shader;
	static Texture* _textureLogo;
	static Texture* _loadingBar[];

	static float _vertices[];
	static unsigned int _indices[];
	static unsigned int _VBO, _IBO;


public:
	static void init();
	static void draw(const int& loadedChunks);

};

#endif // LOADING_SCREEN_H
