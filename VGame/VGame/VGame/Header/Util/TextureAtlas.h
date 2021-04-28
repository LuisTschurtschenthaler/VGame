#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include <string>
#include <array>
#include <GLM/glm.hpp>
#include "Texture.h"


class TextureAtlas {

private:
	static int _atlasSize, _tileSize;
	Texture* _textureAtlas;


public:
	TextureAtlas(const char* texturePath, int textureUnit);
	~TextureAtlas();
	
	static glm::vec2 getTextureCoords(const glm::vec2& uv, const int& textureID);

	Texture& getTexture() { return *_textureAtlas; }
	const int getAtlasSize() const { return _atlasSize; }
	const int getTileSize() const { return _tileSize; }

};

#endif // TEXTURE_ATLAS_H
