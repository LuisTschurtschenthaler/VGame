#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <string>
#include "TerrainGenerator.h"
#include "Chunk.h"
class Game;
class Shader;
class Player;
class ChunkManager;
class TextureAtlas;

class World {

public:
	TerrainGenerator* terrainGenerator;
	bool disposed;

private:
	Game* _game;
	Shader* _chunkShader;
	Shader* _skyShader;
	Player* _player;
	ChunkManager* _chunkManager;
	TextureAtlas* _textureAtlas;
	
	std::string _worldName;


public:
	World(Game* game, TerrainGeneratorType type, const std::string& worldName);
	~World();

	void update();
	void draw();
	
	Player* getPlayer() { return _player; }
	ChunkManager* getChunkManager() { return _chunkManager; }
	TextureAtlas* getTextureAtlas() { return _textureAtlas; }
	const std::string& const getName() { return _worldName; }

private:
	void _updateUniforms();

};

#endif // WORLD_H
