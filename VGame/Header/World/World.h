#ifndef WORLD_H
#define WORLD_H

#include "TerrainGenerator.h"

class Skybox;
class Player;
class ChunkManager;

class World {

public:
	static TerrainGenerator* terrainGenerator;
	static bool gravityEnabled, disposed;

private:
	static Skybox* _skybox;
	static Player* _player;
	static ChunkManager* _chunkManager;


public:
	World();
	~World();

	void update();
	void draw();
	
	static Player& getPlayer() { return *_player; }
	static ChunkManager& getChunkManager() { return *_chunkManager; }

};

#endif // WORLD_H
