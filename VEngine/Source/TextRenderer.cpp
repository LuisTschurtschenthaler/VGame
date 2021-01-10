#include <cmath>
#include "TextRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Window.h"
#include "Log.h"
#include "Timer.h"
#include "Game.h"
#include "Player.h"
#include "ChunkMesh.h"
#include "ChunkManager.h"
#include "FramerateCounter.h"
#include "Util.h"


bool TextRenderer::shouldShowText = false;
Key TextRenderer::key = Key(KeyCode::KEY_F3);


TextRenderer::TextRenderer() {
	_shader = new Shader("text_vert.glsl", "text_frag.glsl");
}

TextRenderer::~TextRenderer() {
}


void TextRenderer::handle() {
	key.update();

	if(key.wasPressedAndReleased()) {
		shouldShowText = !shouldShowText;
		Game::debugMode = !Game::debugMode;
	}
}

void TextRenderer::init(std::string fontPath, int fontSize) {
	if(FT_Init_FreeType(&_freeType))
		Log::write(Log::ERROR, "Couldn't init FreeType library");

	if(FT_New_Face(_freeType, fontPath.c_str(), 0, &_face))
		Log::write(Log::ERROR, "Failed to load font: " + fontPath);

	FT_Set_Pixel_Sizes(_face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(unsigned char c = 0; c < 128; c++) {
		if(FT_Load_Char(_face, c, FT_LOAD_RENDER)) {
			Log::write(Log::ERROR, "Failed to load Glyph");
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED,
			_face->glyph->bitmap.width,
			_face->glyph->bitmap.rows,
			0, GL_RED,GL_UNSIGNED_BYTE,
			_face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
			glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
			_face->glyph->advance.x
		};
		_characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(_face);
	FT_Done_FreeType(_freeType);

	glGenVertexArrays(1, &_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
}

void TextRenderer::update(Player* player) {
	if(!shouldShowText)
		return;

	_gameInfos.clear();
	_gameInfos.shrink_to_fit();

	glm::vec3 playerPos = player->position;

	std::string fps = std::to_string(std::ceil(FramerateCounter::getFPS()));
	std::string title = "VGame FPS: " + fps.substr(0, fps.find("."));

	std::string time = "Time: " + std::to_string(Game::dayTime) + " " + Util::angleToTime(Game::dayTime);

	std::string pitch = std::to_string(player->pitch);
	std::string yaw = std::to_string(player->yaw);
	std::string facing = "Facing: "
		+ Util::yawToCardinalPoint(player->yaw)
		+ " ("  + pitch.substr(0, pitch.find(".") + 2)
		+ " / " + yaw.substr(0, yaw.find(".") + 2)
		+ ")";

	std::string positionXYZ = "XYZ: " 
				+ std::to_string(playerPos.x) 
		+ " / " + std::to_string(playerPos.y) 
		+ " / " + std::to_string(playerPos.z);
	
	std::string blockPositionXYZ = "Block: " 
				+ std::to_string(static_cast<int>(std::ceil(playerPos.x))) 
		+ " / " + std::to_string(static_cast<int>(std::ceil(playerPos.y)))
		+ " / " + std::to_string(static_cast<int>(std::ceil(playerPos.z)));
	
	std::string chunkPositionXYZ = "Chunk: " 
				+ std::to_string(static_cast<int>(std::floor(playerPos.x / CHUNK_SIZE))) 
		+ " / " + std::to_string(static_cast<int>(std::floor(playerPos.y / CHUNK_SIZE)))
		+ " / " + std::to_string(static_cast<int>(std::floor(playerPos.z / CHUNK_SIZE)));

	//std::string biome = "Biome: " + World::terrainGenerator->getBiomeAt()

	std::string vertices = "Vertices: " + std::to_string(ChunkMesh::amountOfVertices);
	std::string indices = "Indices: " + std::to_string(ChunkMesh::amountOfIndices);

	_gameInfos = {
		GameInfo(title,				15.f, Window::getHeight() - 30.f),
		GameInfo(time,				15.f, Window::getHeight() - 60.f),
		GameInfo("",				15.f, Window::getHeight() - 90.f),
		GameInfo(facing,			15.f, Window::getHeight() - 120.f),
		GameInfo(positionXYZ,		15.f, Window::getHeight() - 150.f), 
		GameInfo(blockPositionXYZ,  15.f, Window::getHeight() - 180.f),
		GameInfo(chunkPositionXYZ,	15.f, Window::getHeight() - 210.f),
		GameInfo("",				15.f, Window::getHeight() - 240.f),
		GameInfo(vertices,			15.f, Window::getHeight() - 270.f),
		GameInfo(indices,			15.f, Window::getHeight() - 300.f)
	};
}



/* Deprecated
void TextRenderer::draw(std::string text, float x, float y, float scale, glm::vec3 color) {
	_shader->bind();

	_shader->setMat4("projection", glm::ortho(0.f, static_cast<float>(Window::getWidth()), 0.f, static_cast<float>(Window::getHeight())));
	_shader->setVec3("textColor", glm::vec3(color));
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	for(std::string::const_iterator c = text.begin(); c != text.end(); c++) {
		Character ch = _characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float vertices[6][4] = {
			{ xpos,     ypos + h, 0.f, 0.f },
			{ xpos,     ypos,     0.f, 1.f },
			{ xpos + w, ypos,     1.f, 1.f },

			{ xpos,     ypos + h, 0.f, 0.f },
			{ xpos + w, ypos,     1.f, 1.f },
			{ xpos + w, ypos + h, 1.f, 0.f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	_shader->unbind();
}
*/

void TextRenderer::draw() {
	if(!shouldShowText) return;

	float scale = 1.f;
	glm::vec3 color = glm::vec3(1.f);

	for(unsigned int i = 0; i < _gameInfos.size(); i++){
		_shader->bind();
		_shader->setMat4("projection", glm::ortho(0.f, static_cast<float>(Window::getWidth()), 0.f, static_cast<float>(Window::getHeight())));
		_shader->setVec3("textColor", glm::vec3(color));
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		float tempX = _gameInfos[i].x;
		float tempY = _gameInfos[i].y;

		for(std::string::const_iterator c = _gameInfos[i].text.begin(); c != _gameInfos[i].text.end(); c++) {
			Character ch = _characters[*c];

			float xpos = tempX + ch.bearing.x * scale;
			float ypos = tempY - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h, 0.f, 0.f },
				{ xpos,     ypos,     0.f, 1.f },
				{ xpos + w, ypos,     1.f, 1.f },

				{ xpos,     ypos + h, 0.f, 0.f },
				{ xpos + w, ypos,     1.f, 1.f },
				{ xpos + w, ypos + h, 1.f, 0.f }
			};

			glBindTexture(GL_TEXTURE_2D, ch.textureID);

			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			tempX += (ch.advance >> 6) * scale;
		}

		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		_shader->unbind();
	}
}
