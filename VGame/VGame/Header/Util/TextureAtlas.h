#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include <string>
#include <GLM/glm.hpp>
#include "Texture.h"
class Shader;

class TextureAtlas {

private:
	static int _atlasSize, _tileSize;
	Texture* _textureAtlas;


public:
	TextureAtlas(const char* texturePath, int textureUnit);
	~TextureAtlas();
	
	static glm::vec2 getTextureCoords(glm::vec2 uv, const int textureID);
	void updateUniforms(Shader* shader);

	unsigned int getID() const { return _textureAtlas->getID(); }
	int getAtlasSize() const { return _atlasSize; }
	int getTileSize() const { return _tileSize; }

};

#endif // TEXTURE_ATLAS_H
