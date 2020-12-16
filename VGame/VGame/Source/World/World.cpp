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


bool World::gravityEnabled = true;


World::World(Game* game, TerrainGeneratorType type, const std::string& worldName)
	: _game(game), _worldName(worldName), disposed(false) {

	terrainGenerator = TerrainGenerator::generators[type];
	
	_player = _game->getPlayer();
	_chunkManager = new ChunkManager(this, terrainGenerator, _player);
	_chunkManager->setPlayerSpawnPoint(*_player);

	_skyShader = new Shader("sky.vert", "sky.frag");
	_textureAtlas = new TextureAtlas("./Resources/Textures/Blocks/Atlas.png", 0);

	_meshShader.insert(_meshShader.begin(), {
		new Shader("ChunkShader/solid.vert", "ChunkShader/solid.frag"),
		new Shader("ChunkShader/flora.vert", "ChunkShader/flora.frag"),
		new Shader("ChunkShader/fluid.vert", "ChunkShader/fluid.frag")
	});
}

World::~World() {
	disposed = true;
}
 

void World::update() {
	_player->input();
	_player->update();
	_player->doCollision();
}

void World::prepareDraw() {
	for(size_t i = 0; i < AMOUNT_OF_MESH_TYPES; i++) {
		_meshShader[i]->bind();
		_updateUniforms(static_cast<MeshType>(i));
		_meshShader[i]->unbind();
	}
}

void World::draw() {
	_chunkManager->chunkMapMutex.lock();

	for(auto& chunk : _chunkManager->getChunksToRender()) {
		for(size_t i = 0; i < AMOUNT_OF_MESH_TYPES; i++) {
			_meshShader[i]->bind();
			if(chunk->meshCollection[i]->indices.size() > 0)
				chunk->draw(i);
			_meshShader[i]->unbind();
		}
	}
	
	_chunkManager->chunkMapMutex.unlock();
	Texture::unbind();
}


void World::_updateUniforms(const MeshType& meshType) {
	_textureAtlas->getTexture().bind();
	_meshShader[meshType]->setInt("textureAtlas", _textureAtlas->getTextureID());
	_meshShader[meshType]->setMat4("projectionView", _player->camera->getProjectionView());

	_meshShader[meshType]->setFloat("dayTime", Game::dayTime);
	_meshShader[meshType]->setVec3("cameraPosition", _player->position);
	_meshShader[meshType]->setInt("renderDistance", RENDER_DISTANCE);

	_meshShader[meshType]->setVec3("light.direction", _game->getSky()->getSunPosition());
	_meshShader[meshType]->setVec3("light.ambient", glm::vec3(0.6f, 0.6f, 0.2f));
	_meshShader[meshType]->setVec3("light.diffuse", glm::vec3(0.5f));
	_meshShader[meshType]->setVec3("light.specular", glm::vec3(0.5f));
	_meshShader[meshType]->setInt("isPlayerUnderwater", _player->isSwimming);


	switch(meshType) {
		case MeshType::SOLID:
			break;

		case MeshType::FLORA:
			break;

		case MeshType::FLUID:
			break;
	}
}
