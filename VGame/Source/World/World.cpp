#include "World.h"
#include "Player.h"
#include "Skybox.h"
#include "ChunkManager.h"
#include "WorldGenerator.h"
#include "ParticleSystem.h"


WorldGenerator* World::worldGenerator = nullptr;
bool World::gravityEnabled = true;
bool World::disposed = false;

Skybox* World::_skybox = nullptr;
Player* World::_player = nullptr;
ChunkManager* World::_chunkManager = nullptr;
ParticleSystem* World::_particleSystem  = nullptr;


World::World() {
	worldGenerator = new WorldGenerator();

	_skybox = new Skybox();
	_player = new Player();
	_chunkManager = new ChunkManager();
	_particleSystem = new ParticleSystem();
}

World::~World() {
	disposed = true;

	delete _particleSystem;
	delete _chunkManager;
	delete _player;
	delete _skybox;
}


void World::update() {
	_player->update();
	_skybox->update();
	_chunkManager->update();
	_particleSystem->update();
}

void World::draw() {
	// draw player/entity models
	//_player->draw();

	_skybox->draw();
	_chunkManager->draw();
	_particleSystem->draw();
}
