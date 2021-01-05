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

	Texture& getTexture() const { return *_textureAtlas; }
	const unsigned int getTextureID() const { return _textureAtlas->getID(); }
	const int getAtlasSize() const { return _atlasSize; }
	const int getTileSize() const { return _tileSize; }

};

#endif // TEXTURE_ATLAS_H
