#include "World.h"
#include "Skybox.h"
#include "Player.h"
#include "ChunkManager.h"


TerrainGenerator* World::terrainGenerator = nullptr;
bool World::gravityEnabled = true;

Skybox* World::_skybox = nullptr;
Player* World::_player = nullptr;
ChunkManager* World::_chunkManager = nullptr;


World::World() {
	terrainGenerator = new TerrainGenerator();

	_skybox = new Skybox();
	_player = new Player();
	_chunkManager = new ChunkManager();
}

World::~World() {
	delete _skybox;
	delete _player;
	delete _chunkManager;
}


void World::update() {
	_player->update();
	_chunkManager->update();
	_skybox->update();
}

void World::draw() {
	// draw player/entity models
	//_player->draw();

	_chunkManager->draw();
	_skybox->draw();
}
