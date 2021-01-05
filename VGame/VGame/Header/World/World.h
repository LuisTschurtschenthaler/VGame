#ifndef WORLD_H
#define WORLD_H

#include "TerrainGenerator.h"

class Sky;
class Player;
class ChunkManager;

class World {

public:
	static bool gravityEnabled;
	static TerrainGenerator* terrainGenerator;
	bool disposed;

private:
	static Sky* _sky;
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
