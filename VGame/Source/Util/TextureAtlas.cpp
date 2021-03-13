#include "TextureAtlas.h"
#include "Texture.h"
#include "Shader.h"


int TextureAtlas::_atlasSize = 256;
int TextureAtlas::_tileSize = 16;


TextureAtlas::TextureAtlas(const char* texturePath, int textureUnit) {
	_textureAtlas = new Texture(texturePath, textureUnit);
}

TextureAtlas::~TextureAtlas() {
}


void TextureAtlas::updateUniforms(Shader* shader) {
	_textureAtlas->bind();

	shader->setInt("textureAtlas", _textureAtlas->getID());
}

glm::vec2 TextureAtlas::getTextureCoords(const glm::vec2& uv, const int& textureID) {
	float x = textureID % _tileSize;
	float y = std::floor(textureID / _tileSize);
	glm::vec2 result(0.f);

	if(uv == glm::vec2(0.f))
		result = glm::vec2((x * _tileSize / _atlasSize), (y * _tileSize / _atlasSize));

	else if(uv == glm::vec2(1.f, 0.f))
		result = glm::vec2(((x + 1.f) * _tileSize / _atlasSize), (y * _tileSize / _atlasSize));

	else if(uv == glm::vec2(1.f))
		result = glm::vec2(((x + 1.f) * _tileSize / _atlasSize), ((y + 1.f) * _tileSize / _atlasSize));

	else if(uv == glm::vec2(0.f, 1.f))
		result = glm::vec2((x * _tileSize / _atlasSize), ((y + 1.f) * _tileSize / _atlasSize));
	return result;
}
