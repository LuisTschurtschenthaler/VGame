#include "TextureAtlas.h"
#include "Texture.h"
#include "Shader.h"


TextureAtlas::TextureAtlas(const char* texturePath, int textureUnit, int atlasSize, int tileSize)
	: _atlasSize(atlasSize), _tileSize(tileSize) {

	_textureAtlas = new Texture(texturePath, textureUnit);
}

TextureAtlas::~TextureAtlas() {
}


void TextureAtlas::updateUniforms(Shader* shader) {
	_textureAtlas->bind();

	shader->setInt("textureAtlas", _textureAtlas->getID());
	shader->setInt("atlasSize", _atlasSize);
	shader->setInt("tileSize", _tileSize);
}
