#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <GLEW/GL/glew.h>
#include "Texture.h"
#include "Log.h"
#include "Constants.h"


Texture::Texture(const std::string& filename, int unit)
	: _unit(unit) {

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	
	std::string path = PATH_TEXTURES + filename;
	unsigned char* data = stbi_load(path.c_str(), &_width, &_height, NULL, STBI_rgb_alpha);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if(data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	} else Log::write(Log::ERROR, "Failed to load texture: " + std::to_string(unit));
	
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

Texture::~Texture() {
	glDeleteTextures(1, &_id);
}


void Texture::bind() {
	glActiveTexture(GL_TEXTURE0 + _unit);
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind() {
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::loadCubemap(std::vector<std::string> textures) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int width, height;
	unsigned char* data;

	for(unsigned int i = 0; i < textures.size(); i++) {
		std::string path = PATH_TEXTURES + textures[i];
		data = stbi_load(path.c_str(), &width, &height, NULL, 0);

		if(data)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else Log::write(Log::ERROR, "Cubemap texture failed to load at path: " + textures[i]);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
}
