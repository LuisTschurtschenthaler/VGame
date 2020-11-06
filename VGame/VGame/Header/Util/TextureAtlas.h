#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include <string>
#include "Texture.h"
class Shader;

class TextureAtlas {

private:
	Texture* _textureAtlas;
	int _atlasSize, _tileSize;


public:
	TextureAtlas(const char* texturePath, int textureUnit, int atlasSize, int tileSize);
	~TextureAtlas();
	
	void updateUniforms(Shader* shader);

	unsigned int getID() const { return _textureAtlas->getID(); }
	int getAtlasSize() const { return _atlasSize; }
	int getTileSize() const { return _tileSize; }

};

#endif // TEXTURE_ATLAS_H
