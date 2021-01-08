#ifndef TEXT_H
#define TEXT_H

#include "Input.h"
#include <string>
#include <vector>
#include <map>
#include <GLEW/GL/glew.h>
#include <GLM/glm.hpp>
#include <FREETYPE/ft2build.h>
#include FT_FREETYPE_H

class Shader;
class Player;
class ChunkManager;


struct Character {
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

struct GameInfo {
	std::string text;
	float x, y;

	GameInfo(std::string text, float x, float y)
		: text(text), x(x), y(y) {
	}
};


class TextRenderer {

public:
	static bool shouldShowText;
	static Key key;

	enum Colors {
		GREEN,		  // &a
		AQUA,		  // &b
		RED,		  // &c
		LIGHT_PURPLE, // &d
		YELLOW,		  // &e
		WHITE,		  // &f
		BLACK,		  // &0
		DARK_BLUE,	  // &1
		DARK_GREEN,	  // &2
		DARK_AQUA,	  // &3
		DARK_RED,	  // &4
		DARK_PURPLE,  // &5
		GOLD,		  // &6
		GRAY,		  // &7
		DARK_GRAY,	  // &8
		BLUE,		  // &9
	};

private:
	std::vector<GameInfo> _gameInfos;
	std::map<GLchar, Character> _characters;
	FT_Library _freeType;
	FT_Face _face;
	Shader* _shader;
	GLuint _VAO, _VBO;


public:
	TextRenderer();
	~TextRenderer();

	static void handle();

	void init(std::string fontPath, int fontSize);
	void update(Player* player);
	//void draw(std::string text, float x, float y, float scale = 1.f, glm::vec3 color = glm::vec3(1.f));
	void draw();

};

#endif // TEXT_H