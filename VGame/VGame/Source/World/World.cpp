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
#include "Raycast.h"


bool World::gravityEnabled = true;


World::World(Game* game, TerrainGeneratorType type, const std::string& worldName)
	: _game(game), _worldName(worldName), disposed(false) {

	terrainGenerator = TerrainGenerator::generators[type];
	
	_player = _game->getPlayer();
	_chunkManager = new ChunkManager(this, terrainGenerator, _player);
	_chunkManager->setPlayerSpawnPoint(*_player);

	_skyShader = new Shader("sky_vert.glsl", "sky_frag.glsl");
	_textureAtlas = new TextureAtlas("./Resources/Textures/Blocks/Atlas.png", 0);

	_meshShader.insert(_meshShader.begin(), {
		new Shader("ChunkShader/solid_vert.glsl", "ChunkShader/solid_frag.glsl"),
		new Shader("ChunkShader/fluid_vert.glsl", "ChunkShader/fluid_frag.glsl")
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
	_meshShader[meshType]->setMat4("projection", _player->camera->getProjection());
	_meshShader[meshType]->setMat4("view", _player->camera->getView());

	_meshShader[meshType]->setFloat("dayTime", Game::dayTime);
	_meshShader[meshType]->setVec3("cameraPosition", _player->position);
	_meshShader[meshType]->setInt("renderDistance", RENDER_DISTANCE);

	_meshShader[meshType]->setVec3("light.direction", _game->getSky()->getSunPosition());
	_meshShader[meshType]->setVec3("light.ambient", glm::vec3(0.6f, 0.6f, 0.2f));
	_meshShader[meshType]->setVec3("light.diffuse", glm::vec3(0.5f));
	_meshShader[meshType]->setVec3("light.specular", glm::vec3(0.5f));
	_meshShader[meshType]->setInt("isPlayerUnderwater", _player->isSwimming);	// <----- ADD POST PROCESSING EFFECT


	switch(meshType) {
		case MeshType::SOLID:
			break;

		case MeshType::FLUID:
			break;
	}
}
