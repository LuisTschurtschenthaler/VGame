#include "World.h"
#include "Sky.h"
#include "Player.h"
#include "ChunkManager.h"


TerrainGenerator* World::terrainGenerator = nullptr;
bool World::gravityEnabled = true;

Sky* World::_sky = nullptr;
Player* World::_player = nullptr;
ChunkManager* World::_chunkManager = nullptr;


World::World() {
	terrainGenerator = new TerrainGenerator();

	//_sky = new Sky();
	_player = new Player();
	_chunkManager = new ChunkManager();
}

World::~World() {
	//delete _sky;
	delete _player;
	delete _chunkManager;
}


void World::update() {
	//_sky->update();
	_player->update();
	_chunkManager->update();
}

void World::draw() {
	// draw player/entity models
	//_player->draw();

	_chunkManager->draw();
	//_sky->draw();
}
