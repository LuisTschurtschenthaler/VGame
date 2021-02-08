#include <map>
#include <algorithm>
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
#include "WorldGenerator.h"


ChunkManager::ChunkManager() {
	_solidShader = new Shader("ChunkShader/solid_vert.glsl", "ChunkShader/solid_frag.glsl");
	_waterShader = new Shader("ChunkShader/fluid_vert.glsl", "ChunkShader/fluid_frag.glsl");
	_textureAtlas = new TextureAtlas("./Resources/Textures/Blocks/Atlas.png", 0);

	_threads.emplace_back([&]() { _generateChunks(); });
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

	std::map<float, Chunk*> sortedChunks = _getSortedCunks(playerX, playerZ);

	_solidShader->bind();
	_textureAtlas->getTexture().bind();
	
	for(auto it = sortedChunks.rbegin(); it != sortedChunks.rend(); it++)
		it->second->drawSolid();
	for(auto it = sortedChunks.rbegin(); it != sortedChunks.rend(); it++)
		it->second->drawTransparent();
	
	_solidShader->unbind();

	for(auto it = sortedChunks.rbegin(); it != sortedChunks.rend(); it++) {
		_waterShader->bind();
		_textureAtlas->getTexture().bind();
		it->second->drawFluid();
		_waterShader->unbind();
	}
}

void ChunkManager::findSpawnPoint(glm::vec3& position) {
	const int c = 100;

	int chunkPosX = Random::get(0, CHUNK_SIZE - 1),
		chunkPosZ = Random::get(0, CHUNK_SIZE - 1);

	Biome* biome = World::worldGenerator->getBiomeAt(chunkPosX, chunkPosZ, { c, c });
	int height = std::ceil(biome->getHeight(chunkPosX, chunkPosZ, c, c));

	height = (height > WATER_LEVEL) ? height : WATER_LEVEL;
	position = { chunkPosX + (c * CHUNK_SIZE) + HALF_BLOCK_SIZE, height + 3, chunkPosZ + (c * CHUNK_SIZE) + HALF_BLOCK_SIZE };
}

void ChunkManager::getNearbyChunks(const ChunkXZ& coord, Chunk** nearbyChunks) {
	nearbyChunks[CHUNK_RIGHT] = getChunk({ coord.x + 1, coord.z });
	nearbyChunks[CHUNK_LEFT] = getChunk({ coord.x - 1, coord.z });
	nearbyChunks[CHUNK_FRONT] = getChunk({ coord.x, coord.z + 1 });
	nearbyChunks[CHUNK_BACK] = getChunk({ coord.x, coord.z - 1 });
}

void ChunkManager::removeBlock(const LocationXYZ& loc) {
	placeBlock(loc, BlockID::AIR);
}

void ChunkManager::placeBlock(const LocationXYZ& loc, BlockID blockID) {
	Chunk* chunk = getChunkFromLocation(loc);
	LocationXYZ blockLoc = getBlockLocation(loc);

	_setNearbyChunksDirty(chunk, blockLoc);
	chunk->chunkData.set(blockLoc, blockID);

	chunk->minimumPoint = std::min(chunk->minimumPoint, loc.y - 1);
	chunk->highestPoint = std::max(chunk->highestPoint, loc.y);
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
	if(loc.x < 0) loc.x += CHUNK_SIZE;
	if(loc.z < 0) loc.z += CHUNK_SIZE;

	return loc;
}

BlockID ChunkManager::getBlockID(const LocationXYZ& location) {
	Chunk* chunk = getChunkFromLocation(location);
	LocationXYZ blockLoc = getBlockLocation(location);

	if(isLocationOutOfChunkRange(blockLoc))
		return BlockID::AIR;

	return chunk->chunkData.get(blockLoc);
}

bool ChunkManager::isLocationOutOfChunkRange(const LocationXYZ& location) {
	return(!(location.x >= 0
		   && location.x < CHUNK_SIZE
		   && location.y >= 0
		   && location.y < CHUNK_HEIGHT
		   && location.z >= 0
		   && location.z < CHUNK_SIZE));
}


std::map<float, Chunk*> ChunkManager::_getSortedCunks(const int& playerX, const int& playerZ) {
	std::map<float, Chunk*> sortedChunks = std::map<float, Chunk*>();
	for(auto it = _chunks.begin(); it != _chunks.end(); it++) {
		if(!it->second->meshesGenerated)
			continue;

		int distanceX = playerX - it->first.x;
		int distanceZ = playerZ - it->first.z;

		if(distanceX < RENDER_DISTANCE && distanceX > -RENDER_DISTANCE &&
		   distanceZ < RENDER_DISTANCE && distanceZ > -RENDER_DISTANCE) {

			glm::vec2 playerPos = { World::getPlayer().position.x, World::getPlayer().position.z };
			glm::vec2 worldCoord = { it->second->worldCoord.x, it->second->worldCoord.z };

			float distance = glm::length(playerPos - worldCoord);
			sortedChunks[distance] = it->second;
		}
	}
	return sortedChunks;
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
	while(true) {
		int currentChunkX = int(World::getPlayer().position.x / CHUNK_SIZE);
		int currentChunkZ = int(World::getPlayer().position.z / CHUNK_SIZE);

		for(int i = 0; i < RENDER_DISTANCE; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			for(int x = currentChunkX - i; x <= currentChunkX + i; x++)
			for(int z = currentChunkZ - i; z <= currentChunkZ + i; z++) {
				if(World::disposed) return;
				if(x <= 0 || z <= 0) continue;

				Chunk* chunk = getChunk({ x, z });
				if(!chunk->chunkDataGenerated)
					World::worldGenerator->generateChunk(*chunk);

				if(chunk->chunkDataGenerated && !chunk->meshesGenerated)
					chunk->generateChunkMesh();
				else if(chunk->isDirty)
					chunk->recreateChunkMesh();
			}
		}
	}
}
