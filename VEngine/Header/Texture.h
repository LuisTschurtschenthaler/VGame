#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <string>

class Texture {

private:
	unsigned int _unit;
	unsigned int _id;
	int _width, _height;


public:
	Texture() { }
	Texture(const char* filename, int unit);
	~Texture();

	void bind();
	
	static void unbind();
	static unsigned int loadCubemap(std::vector<std::string> textures);

	unsigned int getID() const { return _unit; }

};

#endif // TEXTURE_H
