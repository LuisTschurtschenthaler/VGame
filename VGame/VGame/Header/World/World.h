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
	std::vector<Shader*> _meshShader;
	Shader* _skyShader;
	Player* _player;
	ChunkManager* _chunkManager;
	TextureAtlas* _textureAtlas;
	
	std::string _worldName;


public:
	World(Game* game, TerrainGeneratorType type, const std::string& worldName);
	~World();

	void update();
	void prepareDraw();
	void draw();

	const std::string& const getName() { return _worldName; }
	
	Game* getGame() const { return _game; };
	Player* getPlayer() const { return _player; }
	ChunkManager* getChunkManager() const { return _chunkManager; }
	TextureAtlas* getTextureAtlas() const { return _textureAtlas; }

private:
	void _updateUniforms(const MeshType& meshType);

};

#endif // WORLD_H
