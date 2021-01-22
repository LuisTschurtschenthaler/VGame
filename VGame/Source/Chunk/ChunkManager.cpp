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
		while(!World::disposed) {
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
	position = { chunkPosX + CHUNK_SIZE + HALF_BLOCK_SIZE, height + 5, chunkPosZ + CHUNK_SIZE + HALF_BLOCK_SIZE };
}

void ChunkManager::getNearbyChunks(const ChunkXZ& coord, Chunk** nearbyChunks) {
	nearbyChunks[CHUNK_RIGHT] = getChunk({ coord.x + 1, coord.z });
	nearbyChunks[CHUNK_LEFT]  = getChunk({ coord.x - 1, coord.z });
	nearbyChunks[CHUNK_FRONT] = getChunk({ coord.x, coord.z + 1 });
	nearbyChunks[CHUNK_BACK]  = getChunk({ coord.x, coord.z - 1 });
}

void ChunkManager::removeBlock(const LocationXYZ& loc) {
	placeBlock(loc, BlockID::AIR);
}

void ChunkManager::placeBlock(const LocationXYZ& loc, BlockID blockID) {
	Chunk* chunk = getChunkFromLocation(loc);
	LocationXYZ blockLoc = getBlockLocation(loc);

	_setNearbyChunksDirty(chunk, blockLoc);
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

	if(isLocationOutOfChunkRange(location))
		return BlockID::AIR;

	return chunk->chunkData.get(blockLoc);
}

bool ChunkManager::isLocationOutOfChunkRange(const LocationXYZ& location) {
	return((location.x < 0 && location.x >= CHUNK_SIZE)
		&& (location.y < 0 && location.y >= CHUNK_HEIGHT)
		&& (location.z < 0 && location.z >= CHUNK_SIZE));
}


void ChunkManager::_setNearbyChunksDirty(Chunk* chunk, const LocationXYZ& location) {
	chunk->isDirty = true;

	if(chunk->nearbyChunks[CHUNK_LEFT] != nullptr && location.x == 0) chunk->nearbyChunks[CHUNK_LEFT]->isDirty = true;
	if(chunk->nearbyChunks[CHUNK_BACK] != nullptr && location.z == 0) chunk->nearbyChunks[CHUNK_BACK]->isDirty = true;
	if(chunk->nearbyChunks[CHUNK_RIGHT] != nullptr && location.x == CHUNK_SIZE - 1) chunk->nearbyChunks[CHUNK_RIGHT]->isDirty = true;
	if(chunk->nearbyChunks[CHUNK_FRONT] != nullptr && location.z == CHUNK_SIZE - 1) chunk->nearbyChunks[CHUNK_FRONT]->isDirty = true;
}

bool ChunkManager::_chunkExists(const ChunkXZ& coord) {
	std::lock_guard<std::mutex> lock(chunkMutex);
	return (_chunks.find(coord) != _chunks.end());
}

void ChunkManager::_generateChunks() {
	int currentChunkX = int(World::getPlayer().position.x / CHUNK_SIZE);
	int currentChunkZ = int(World::getPlayer().position.z / CHUNK_SIZE);

	for(int i = 0; i < RENDER_DISTANCE; i++)
	for(int x = currentChunkX - i; x <= currentChunkX + i; x++)
	for(int z = currentChunkZ - i; z <= currentChunkZ + i; z++) {
		if(x <= 0 || z <= 0) continue;
		//if(x >= 5 || z >= 5) continue;
		
		Chunk* chunk = getChunk({ x, z });
		if(!chunk->chunkDataGenerated)
			chunk->generateChunkData();

		if(!chunk->meshesGenerated && chunk->chunkDataGenerated)
			chunk->generateChunkMesh();

		if(chunk->isDirty)
			chunk->recreateChunkMesh();
	}
}
