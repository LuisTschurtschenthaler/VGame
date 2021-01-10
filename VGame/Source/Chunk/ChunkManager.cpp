#include "ChunkManager.h"
#include "Shader.h"
#include "World.h"
#include "Player.h"
#include "Constants.h"
#include "Game.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Random.h"
#include "World.h"
#include "Biome.h"
#include "TerrainGenerator.h"


ChunkManager::ChunkManager() {
	_solidShader = new Shader("ChunkShader/solid_vert.glsl", "ChunkShader/solid_frag.glsl");
	_waterShader = new Shader("ChunkShader/fluid_vert.glsl", "ChunkShader/fluid_frag.glsl");
	_textureAtlas = new TextureAtlas("./Resources/Textures/Blocks/Atlas.png", 0);

	_threads.emplace_back([&]() {
		while(true) {
			std::this_thread::sleep_for(std::chrono::microseconds(50));
			_generateChunks();
		}
	});
}

ChunkManager::~ChunkManager() {
	for(auto& thread : _threads)
		thread.join();

	delete _solidShader;
	delete _waterShader;
	delete _textureAtlas;
}


void ChunkManager::update() {
	_solidShader->setInt("textureAtlas", _textureAtlas->getTextureID());
	_solidShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_solidShader->setMat4("view", World::getPlayer().camera->getView());

	_waterShader->setInt("textureAtlas", _textureAtlas->getTextureID());
	_waterShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_waterShader->setMat4("view", World::getPlayer().camera->getView());
}

void ChunkManager::draw() {
	int playerX = int(World::getPlayer().position.x / CHUNK_SIZE),
		playerZ = int(World::getPlayer().position.z / CHUNK_SIZE);

	for(auto& it : _chunks) {
		if(!it.second->meshesGenerated)
			continue;

		if(it.first.x < playerX - RENDER_DISTANCE || it.first.x > playerX + RENDER_DISTANCE ||
		   it.first.z < playerZ - RENDER_DISTANCE || it.first.z > playerZ + RENDER_DISTANCE) {

			/* Unload chunk and save to file, just if chunk changed */
			//FileManager::save(*it.second);

		}
		else {
			// Draw fluid
			_waterShader->bind();
			_textureAtlas->getTexture().bind();
			it.second->drawFluid();
			_waterShader->unbind();

			// Draw solid
			_solidShader->bind();
			_textureAtlas->getTexture().bind();
			it.second->drawSolid();
			_solidShader->unbind();
		}
	}
}

void ChunkManager::findSpawnPoint(glm::vec3& position) {
	int chunkPosX = Random::get(0, CHUNK_SIZE - 1),
		chunkPosZ = Random::get(0, CHUNK_SIZE - 1);

	Biome* biome = World::terrainGenerator->getBiomeAt(chunkPosX, chunkPosZ, { 0, 0 });
	int height = std::ceil(biome->getHeight(chunkPosX, chunkPosZ, 0, 0));

	height = (height > WATER_LEVEL) ? height : WATER_LEVEL;
	position = { chunkPosX + CHUNK_SIZE + HALF_BLOCK_SIZE, height + 3, chunkPosZ + CHUNK_SIZE + HALF_BLOCK_SIZE };
}

void ChunkManager::getNearbyChunks(const ChunkXZ& coord, Chunk** nearbyChunks) {
	nearbyChunks[CHUNK_RIGHT] = getChunk({ coord.x + 1, coord.z });
	nearbyChunks[CHUNK_LEFT]  = getChunk({ coord.x - 1, coord.z });
	nearbyChunks[CHUNK_FRONT] = getChunk({ coord.x, coord.z + 1 });
	nearbyChunks[CHUNK_BACK]  = getChunk({ coord.x, coord.z - 1 });
}

void ChunkManager::removeBlock(const LocationXYZ& loc) {
	Chunk* chunk = getChunkFromLocation(loc);
	LocationXYZ blockLoc = getBlockLocation(loc);

	chunk->chunkData.set(blockLoc, BlockID::AIR);
}

void ChunkManager::placeBlock(const LocationXYZ& loc, BlockID blockID) {
	Chunk* chunk = getChunkFromLocation(loc);
	LocationXYZ blockLoc = getBlockLocation(loc);

	chunk->chunkData.set(blockLoc, blockID);
}

Chunk* ChunkManager::getChunk(const ChunkXZ& coord) {
	if(!_chunkExists(coord)) {
		std::lock_guard<std::mutex> lock(chunkMutex);
		_chunks.emplace(coord, new Chunk(this, coord));
	}

	return _chunks[coord];
}

Chunk* ChunkManager::getChunkFromLocation(const LocationXYZ& location) {
	ChunkXZ coord = { location.x / CHUNK_SIZE, location.z / CHUNK_SIZE };
	if(coord.x <= 0) coord.x -= 1;
	if(coord.z <= 0) coord.z -= 1;

	return getChunk(coord);
}

LocationXYZ ChunkManager::getBlockLocation(const LocationXYZ& location) {
	LocationXYZ loc = { location.x % CHUNK_SIZE, location.y, location.z % CHUNK_SIZE };
	if(location.x < 0) loc.x += CHUNK_SIZE;
	if(location.z < 0) loc.z += CHUNK_SIZE;

	return loc;
}

BlockID ChunkManager::getBlockID(const LocationXYZ& location) {
	Chunk* chunk = getChunkFromLocation(location);
	LocationXYZ blockLoc = getBlockLocation(location);

	return chunk->chunkData.get(blockLoc);
}


bool ChunkManager::_chunkExists(const ChunkXZ& coord) {
	std::lock_guard<std::mutex> lock(chunkMutex);
	return (_chunks.find(coord) != _chunks.end());
}

void ChunkManager::_generateChunks() {
	std::this_thread::sleep_for(std::chrono::microseconds(50));
	
	int currentChunkX = int(World::getPlayer().position.x / CHUNK_SIZE);
	int currentChunkZ = int(World::getPlayer().position.z / CHUNK_SIZE);

	for(int i = 0; i < RENDER_DISTANCE; i++)
	for(int x = currentChunkX - i; x <= currentChunkX + i; x++)
	for(int z = currentChunkZ - i; z <= currentChunkZ + i; z++) {
		if(x <= 0 || z <= 0) continue;
		//if(x >= 4 || z >= 4) continue;
		
		Chunk* chunk = getChunk({ x, z });
		if(!chunk->chunkDataGenerated)
			chunk->generateChunkData();

		if(!chunk->meshesGenerated && chunk->chunkDataGenerated)
			chunk->generateChunkMesh();
	}
}
