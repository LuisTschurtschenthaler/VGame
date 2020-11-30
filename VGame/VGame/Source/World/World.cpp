#include <map>
#include <GLM/glm.hpp>
#include <SDL2/SDL.h>
#include "World.h"
#include "Game.h"
#include "Camera.h"
#include "Shader.h"
#include "WorldConstants.h"
#include "ChunkManager.h"
#include "ChunkMesh.h"
#include "Coordinates.h"
#include "Timer.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "MeshTypes.h"
#include "Player.h"
#include "Sky.h"


World::World(Game* game, TerrainGeneratorType type, const std::string& worldName)
	: _game(game), _worldName(worldName), disposed(false) {

	terrainGenerator = TerrainGenerator::generators[type];
	
	_player = _game->getPlayer();
	_chunkManager = new ChunkManager(this, terrainGenerator, _player->camera);
	_chunkManager->setPlayerSpawnPoint(*_player);

	_chunkShader = new Shader("ChunkShader/solid.vert", "ChunkShader/solid.frag");
	_skyShader = new Shader("sky.vert", "sky.frag");
	_textureAtlas = new TextureAtlas("./Resources/Textures/Blocks/Atlas.png", 0);
}

World::~World() {
	disposed = true;
}
 

void World::update() {
	_player->update(_chunkManager);
}

void World::draw() {
	_chunkShader->bind();
	_updateUniforms();
	_chunkManager->chunkMapMutex.lock();
	
	for(auto& chunk : _chunkManager->getChunksToRender()) {
		for(size_t i = 0; i < AMOUNT_OF_MESH_TYPES; i++) {
			
			if(chunk->meshCollection[i]->indices.size() > 0)
				chunk->draw(i);

		}
	}
	
	_chunkManager->chunkMapMutex.unlock();
	_chunkShader->unbind();
	Texture::unbind();
}


void World::_updateUniforms() {
	_textureAtlas->updateUniforms(_chunkShader);
	_chunkShader->setMat4("projectionView", _player->camera->getProjectionView());

	_chunkShader->setFloat("dayTime", Game::dayTime);
	_chunkShader->setVec3("cameraPosition", _player->getPosition()); 
	_chunkShader->setInt("renderDistance", RENDER_DISTANCE);

	_chunkShader->setVec3("light.direction", _game->getSky()->getSunPosition());
	_chunkShader->setVec3("light.ambient", glm::vec3(0.6f, 0.6f, 0.2f));
	_chunkShader->setVec3("light.diffuse", glm::vec3(0.5f));
	_chunkShader->setVec3("light.specular", glm::vec3(0.5f));
}
