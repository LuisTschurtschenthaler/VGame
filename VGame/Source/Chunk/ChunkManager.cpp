#include <map>
#include <vector>
#include <stack>
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
#include "Util.h"
#include "WorldGenerator.h"


ChunkManager::ChunkManager() {
	_solidShader = new Shader("ChunkShader/solid_vert.glsl", "ChunkShader/solid_frag.glsl");
	_waterShader = new Shader("ChunkShader/fluid_vert.glsl", "ChunkShader/fluid_frag.glsl");
	_textureAtlas = new TextureAtlas("Blocks/Atlas.png", 0);

	_threads.emplace_back([&]() { _generateChunks(); });
}

ChunkManager::~ChunkManager() {
	for(auto& thread : _threads)
		thread.join();

	for(auto& chunk : _chunks)
		delete chunk.second;

	delete _textureAtlas;
	delete _waterShader;
	delete _solidShader;
}


void ChunkManager::update() {
	_solidShader->setInt("textureAtlas", _textureAtlas->getTextureID());
	_solidShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_solidShader->setMat4("view", World::getPlayer().camera->getView());
	_solidShader->setVec3("playerPosition", World::getPlayer().position);
	_solidShader->setInt("renderDistance", (RENDER_DISTANCE * CHUNK_SIZE));

	_waterShader->setInt("textureAtlas", _textureAtlas->getTextureID());
	_waterShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_waterShader->setMat4("view", World::getPlayer().camera->getView());
	_waterShader->setVec3("playerPosition", World::getPlayer().position);
	_waterShader->setInt("renderDistance", (RENDER_DISTANCE * CHUNK_SIZE));
}

void ChunkManager::draw() {
	int playerX = int(World::getPlayer().position.x / CHUNK_SIZE),
		playerZ = int(World::getPlayer().position.z / CHUNK_SIZE);

	std::vector<Chunk*> sortedChunks = _getSortedCunks(playerX, playerZ);

	_solidShader->bind();
	_textureAtlas->getTexture().bind();
	

	glEnable(GL_CULL_FACE);
	for(auto& chunk : sortedChunks)
		chunk->drawSolid();
	for(auto& chunk : sortedChunks)
		chunk->drawTransparent();
	
	_solidShader->unbind();


	glDisable(GL_CULL_FACE);
	for(auto& chunk : sortedChunks) {
		_waterShader->bind();
		_textureAtlas->getTexture().bind();
		chunk->drawFluid();
		_waterShader->unbind();
	}
}

void ChunkManager::findSpawnPoint(Entity& entity) {
	int attempts = 0;
	int height = 0;

	int chunkX, chunkZ, chunkPosX, chunkPosZ;
	do {
		chunkX = Random::getIntInRange(100, 200);
		chunkZ = Random::getIntInRange(100, 200);
		chunkPosX = Random::get(0, CHUNK_SIZE - 1);
		chunkPosZ = Random::get(0, CHUNK_SIZE - 1);

		Biome* biome = World::worldGenerator->getBiomeAt(chunkPosX, chunkPosZ, { chunkX, chunkZ });
		height = std::ceil(biome->getHeight(chunkPosX, chunkPosZ, chunkX, chunkZ));
		
		attempts++;
	} while(height <= WATER_LEVEL);

	int worldPosX = chunkPosX + chunkX * CHUNK_SIZE,
		worldPosZ = chunkPosZ + chunkZ * CHUNK_SIZE;

	entity.position = { worldPosX + 0.5f, height + 5.f, worldPosZ + 0.5f };
	std::cout << "Attempts for spawn finding: " << attempts << std::endl;
	std::cout << "Spawnlocation fount at: " << entity.position.x << " " << entity.position.y << " " << entity.position.z << std::endl;
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

void ChunkManager::placeBlock(const LocationXYZ& loc, const BlockID& blockID) {
	Chunk* chunk = getChunkFromLocation(loc);
	LocationXYZ blockLoc = getBlockLocation(loc);

	if(isLocationOutOfChunkRange(blockLoc))
		return;

	_setNearbyChunksDirty(chunk, blockLoc);
	_setNearbyChunksMinMax(chunk, loc.y - 1, loc.y + 1);
	chunk->chunkData.set(blockLoc, blockID);
}

void ChunkManager::replaceBlock(const LocationXYZ& location, const BlockID& blockToReplace, const BlockID& block) {
	if(getBlockID(location) == blockToReplace) {
		Chunk* chunk = getChunkFromLocation(location);
		LocationXYZ blockLoc = getBlockLocation(location);

		chunk->chunkData.set(blockLoc, block);
	}
}

Chunk* ChunkManager::getChunk(const ChunkXZ& coord) {
	if(!_chunkExists(coord)) {
		std::lock_guard<std::mutex> lock(chunkMutex);
		_chunks.emplace(coord, new Chunk(coord));
	}

	return _chunks[coord];
}

Chunk* ChunkManager::getChunkFromLocation(const LocationXYZ& location) {
	ChunkXZ coord = { location.x / CHUNK_SIZE, location.z / CHUNK_SIZE };
	return getChunk(coord);
}

const LocationXYZ ChunkManager::getBlockLocation(const LocationXYZ& location) const {
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

const bool ChunkManager::isLocationOutOfChunkRange(const LocationXYZ& location) {
	return !(location.x >= 0 && location.y >= 0 && location.z >= 0 
		   && location.x <= CHUNK_SIZE - 1
		   && location.y <= CHUNK_HEIGHT - 2
		   && location.z <= CHUNK_SIZE - 1);
}


std::vector<Chunk*> ChunkManager::_getSortedCunks(const int& playerX, const int& playerZ) {
	std::vector<Chunk*> sortedChunks;
	std::stack<ChunkXZ> chunksToDelete;

	std::unordered_map<ChunkXZ, Chunk*>::iterator it = _chunks.begin();
	for(; it != _chunks.end(); ) {
		if(!it->second->meshesGenerated) {
			it++;
			continue;
		}

		int distanceX = playerX - it->first.x;
		int distanceZ = playerZ - it->first.z;

		if(distanceX < RENDER_DISTANCE && distanceX > -RENDER_DISTANCE &&
		   distanceZ < RENDER_DISTANCE && distanceZ > -RENDER_DISTANCE) {

			sortedChunks.push_back(it->second);
		}
		else if(distanceX >= DESTROY_DISTANCE || distanceX <= -DESTROY_DISTANCE ||
				distanceZ >= DESTROY_DISTANCE || distanceZ <= -DESTROY_DISTANCE) {

			chunksToDelete.push(it->second->coord);
		}
		it++;
	}

	while(!chunksToDelete.empty()) {
		ChunkXZ top = chunksToDelete.top();
		
		delete _chunks[top];
		_chunks[top] = nullptr;
		
		_chunks.erase(top);
		chunksToDelete.pop();
	}

	glm::vec2 playerPos = { World::getPlayer().position.x, World::getPlayer().position.z };

	std::sort(sortedChunks.begin(), sortedChunks.end(), 
		[=](const Chunk* ch1, const Chunk* ch2) {
			glm::vec2 d1 = { ch1->worldCoord.x, ch1->worldCoord.z },
					  d2 = { ch2->worldCoord.x, ch2->worldCoord.z };
			
			return (Util::getDistance(playerPos, d1) < Util::getDistance(playerPos, d2));
		}
	);
	return sortedChunks;
}

void ChunkManager::_setNearbyChunksDirty(Chunk* chunk, const LocationXYZ& location) {
	auto setNearbyChunkDirty = [](Chunk* nearbyChunk, bool locationCheck) {
		if(nearbyChunk != nullptr && locationCheck)
			nearbyChunk->isDirty = true;
	};

	chunk->isDirty = true;
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_LEFT], (location.x == 0));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_BACK], (location.z == 0));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_RIGHT], (location.x == CHUNK_SIZE - 1));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_FRONT], (location.z == CHUNK_SIZE - 1));
}

void ChunkManager::_setNearbyChunksMinMax(Chunk* chunk, const int& min, const int& max) {
	auto setMinMax = [&](Chunk* nearbyChunk) {
		if(nearbyChunk != nullptr) {
			nearbyChunk->minimumPoint = std::min(nearbyChunk->minimumPoint, (min < 0) ? 0 : min);
			nearbyChunk->highestPoint = std::max(nearbyChunk->highestPoint, (max > CHUNK_HEIGHT - 1) ? CHUNK_HEIGHT - 1 : max);
		}
	};

	setMinMax(chunk);
	setMinMax(chunk->nearbyChunks[CHUNK_LEFT]);
	setMinMax(chunk->nearbyChunks[CHUNK_BACK]);
	setMinMax(chunk->nearbyChunks[CHUNK_RIGHT]);
	setMinMax(chunk->nearbyChunks[CHUNK_FRONT]);
}

bool ChunkManager::_chunkExists(const ChunkXZ& coord) {
	std::lock_guard<std::mutex> lock(chunkMutex);
	return (_chunks.find(coord) != _chunks.end());
}

void ChunkManager::_generateChunks() {
	while(true) {
		int currentChunkX = int(World::getPlayer().position.x / CHUNK_SIZE);
		int currentChunkZ = int(World::getPlayer().position.z / CHUNK_SIZE);

		for(int i = 0; i <= RENDER_DISTANCE; i++)
		for(int x = currentChunkX - i; x < currentChunkX + i; x++)
		for(int z = currentChunkZ - i; z < currentChunkZ + i; z++) {
			if(World::disposed) return;
			if(x <= 0 || z <= 0) continue;

			Chunk* chunk = getChunk({ x, z });
			if(!chunk->chunkDataGenerated)
				World::worldGenerator->generateChunk(*chunk);

			if(!chunk->meshesGenerated)
				chunk->generateChunkMesh();
			else if(chunk->isDirty)
				chunk->recreateChunkMesh();
		}
	}
}
