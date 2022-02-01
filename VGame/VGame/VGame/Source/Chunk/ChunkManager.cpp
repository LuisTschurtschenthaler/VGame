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
	_chunkShader = new Shader("chunk_vert.glsl", "chunk_frag.glsl");
	_textureAtlas = new TextureAtlas("Blocks/Atlas.png", 0);

	_threads.emplace_back([&]() { _generateChunks(); });
}

ChunkManager::~ChunkManager() {
	for(auto& thread : _threads)
		thread.join();

	for(auto& chunkArea : _chunkAreas)
		delete chunkArea.second;
	
	delete _textureAtlas;
	delete _chunkShader;
}


void ChunkManager::update() {
	_chunkShader->setInt("textureAtlas", 0);
	_chunkShader->setMat4("projection", World::getPlayer().camera->getProjection());
	_chunkShader->setMat4("view", World::getPlayer().camera->getView());
	_chunkShader->setVec3("playerPosition", World::getPlayer().position);
	_chunkShader->setInt("renderDistance", (RENDER_DISTANCE * CHUNK_SIZE));
}

void ChunkManager::draw() {
	int playerX = int(World::getPlayer().position.x / CHUNK_SIZE),
		playerY = int(World::getPlayer().position.x / CHUNK_SIZE),
		playerZ = int(World::getPlayer().position.z / CHUNK_SIZE);

	std::vector<Chunk*> sortedChunks = _getSortedCunks(playerX, playerY, playerZ);

	glEnable(GL_CULL_FACE);
	_chunkShader->bind();
	_textureAtlas->getTexture().bind();

	for(auto& chunk : sortedChunks) {
		glm::vec3 worldCoord = glm::vec3(chunk->worldCoord.x, chunk->worldCoord.y, chunk->worldCoord.z);
		_chunkShader->setVec3("chunkWorldCoord", worldCoord);
		chunk->drawSolid();
	}

	glDisable(GL_CULL_FACE);
	for(auto& chunk : sortedChunks) {
		glm::vec3 worldCoord = glm::vec3(chunk->worldCoord.x, chunk->worldCoord.y, chunk->worldCoord.z);
		_chunkShader->setVec3("chunkWorldCoord", worldCoord);
		chunk->drawFluid();
	}

	_chunkShader->unbind();
}

void ChunkManager::findSpawnPoint(Entity& entity) {
	int attempts = 0;
	uint8_t height = 0;

	int32_t chunkX, chunkZ, chunkPosX, chunkPosZ;
	do {
		chunkX = Random::getIntInRange(100, 200);
		chunkZ = Random::getIntInRange(100, 200);
		chunkPosX = Random::get(0, CHUNK_SIZE - 1);
		chunkPosZ = Random::get(0, CHUNK_SIZE - 1);

		Biome* biome = World::worldGenerator->getBiomeAt(chunkPosX, chunkPosZ, { chunkX, chunkZ });
		height = std::ceil(biome->getHeight(chunkPosX, chunkPosZ, chunkX, chunkZ));

		attempts++;
	} while(height <= WATER_LEVEL);

	int32_t worldPosX = chunkPosX + chunkX * CHUNK_SIZE,
			worldPosZ = chunkPosZ + chunkZ * CHUNK_SIZE;

	entity.position = { worldPosX + 0.5f, height + 5.f, worldPosZ + 0.5f };
	std::cout << "Attempts for spawn finding: " << attempts << std::endl;
	std::cout << "Spawnlocation fount at: " << entity.position.x << " " << entity.position.y << " " << entity.position.z << std::endl;
}

void ChunkManager::getNearbyChunks(const ChunkXYZ& coord, Chunk** nearbyChunks) {
	nearbyChunks[CHUNK_RIGHT]	= getChunk({ coord.x + 1, coord.y, coord.z });
	nearbyChunks[CHUNK_LEFT]	= getChunk({ coord.x - 1, coord.y, coord.z });
	nearbyChunks[CHUNK_TOP]		= getChunk({ coord.x, coord.y + 1, coord.z });
	nearbyChunks[CHUNK_BOTTOM]	= getChunk({ coord.x, coord.y - 1, coord.z });
	nearbyChunks[CHUNK_FRONT]	= getChunk({ coord.x, coord.y, coord.z + 1 });
	nearbyChunks[CHUNK_BACK]	= getChunk({ coord.x, coord.y, coord.z - 1 });
}

void ChunkManager::getNearbyChunkAreas(const ChunkXZ& coord, ChunkArea** nearbyChunks) {
	nearbyChunks[CHUNK_AREA_RIGHT]	= getChunkArea({ coord.x + 1, coord.z });
	nearbyChunks[CHUNK_AREA_LEFT]	= getChunkArea({ coord.x - 1, coord.z });
	nearbyChunks[CHUNK_AREA_FRONT]	= getChunkArea({ coord.x, coord.z + 1 });
	nearbyChunks[CHUNK_AREA_BACK]	= getChunkArea({ coord.x, coord.z - 1 });
}

void ChunkManager::placeBlock(const LocationXYZ& loc, const BlockID& blockID) {
	Chunk* chunk = getChunkFromLocation(loc);
	ChunkArea* chunkArea = getChunkArea({ chunk->coord.x, chunk->coord.z });
	LocationXYZ blockLoc = getBlockLocation(loc);

	if(isLocationOutOfChunkRange(blockLoc))
		return;

	_setNearbyChunksDirty(chunk, blockLoc);
	_setNearbyChunkAreasMinMax(chunkArea, loc.y - 1, loc.y + 1);
	chunk->chunkData.set(blockLoc, blockID);
}

void ChunkManager::replaceBlock(const LocationXYZ& location, const BlockID& blockToReplace, const BlockID& block) {
	if(getBlockID(location) == blockToReplace) {
		Chunk* chunk = getChunkFromLocation(location);
		LocationXYZ blockLoc = getBlockLocation(location);

		chunk->chunkData.set(blockLoc, block);
	}
}

void ChunkManager::removeBlock(const LocationXYZ& loc) {
	placeBlock(loc, BlockID::AIR);
}

ChunkArea* ChunkManager::getChunkArea(const ChunkXZ& coord) {
	if(!_chunkAreaExists(coord)) {
		std::lock_guard<std::mutex> lock(chunkAreaMutex);
		_chunkAreas.emplace(coord, new ChunkArea(coord));
	}

	return _chunkAreas[coord];
}


Chunk* ChunkManager::getChunk(const ChunkXYZ& coord) {
	ChunkArea* chunkArea = getChunkArea({ coord.x, coord.z });
	return ((coord.y <= (chunkArea->highestPoint / CHUNK_SIZE) && coord.y >= 0) ? chunkArea->getChunk(coord.y) : nullptr);
}

Chunk* ChunkManager::getChunkFromLocation(const LocationXYZ& location) {
	ChunkXZ coord = { location.x / CHUNK_SIZE, location.z / CHUNK_SIZE };
	return getChunkArea(coord)->getChunk(location.y / CHUNK_SIZE);
}

LocationXYZ ChunkManager::getBlockLocation(const LocationXYZ& location) {
	LocationXYZ loc = { location.x % CHUNK_SIZE, location.y % CHUNK_SIZE, location.z % CHUNK_SIZE };
	if(loc.x < 0) loc.x += CHUNK_SIZE;
	if(loc.y < 0) loc.y += CHUNK_SIZE;
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

const int ChunkManager::getAmountOfLoadedChunks() {
	int am = 0;
	for (auto it : _chunkAreas) am += it.second->chunks.size();
	return am;
}

const bool ChunkManager::isLocationOutOfChunkRange(const LocationXYZ& location) {
	return !(location.x >= 0 && location.y >= 0 && location.z >= 0
			 && location.x <= CHUNK_SIZE - 1
			 && location.y <= CHUNK_SIZE - 1
			 && location.z <= CHUNK_SIZE - 1);
}


std::vector<Chunk*> ChunkManager::_getSortedCunks(const int& playerX, const int& playerY, const int& playerZ) {
	std::vector<Chunk*> sortedChunks;
	std::stack<ChunkXZ> chunkAreasToDelete;

	std::unordered_map<ChunkXZ, ChunkArea*>::iterator it = _chunkAreas.begin();
	while(it != _chunkAreas.end()) {
		int distanceX = playerX - it->first.x;
		int distanceZ = playerZ - it->first.z;

		if(distanceX >= DESTROY_DISTANCE || distanceX <= -DESTROY_DISTANCE ||
			distanceZ >= DESTROY_DISTANCE || distanceZ <= -DESTROY_DISTANCE) {

			chunkAreasToDelete.push(it->second->coord);
		}
		else {
			for(auto it2 : it->second->chunks) {
				int distanceY = playerY - it2.first.y;

				if(it2.second->meshesGenerated &&
				   distanceX < RENDER_DISTANCE && distanceX > -RENDER_DISTANCE &&
				   distanceZ < RENDER_DISTANCE && distanceZ > -RENDER_DISTANCE) {

						sortedChunks.push_back(it2.second);
				}
			}
		}
		it++;
	}

	while(!chunkAreasToDelete.empty()) {
		ChunkXZ top = chunkAreasToDelete.top();

		delete _chunkAreas[top];
		_chunkAreas[top] = nullptr;

		_chunkAreas.erase(top);
		chunkAreasToDelete.pop();
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
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_LEFT],	(location.x == 0));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_BOTTOM],  (location.y == 0));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_BACK],	(location.z == 0));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_RIGHT], (location.x == CHUNK_SIZE - 1));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_TOP],	  (location.y == CHUNK_SIZE - 1));
	setNearbyChunkDirty(chunk->nearbyChunks[CHUNK_FRONT], (location.z == CHUNK_SIZE - 1));
}

void ChunkManager::_setNearbyChunkAreasMinMax(ChunkArea* chunkArea, const int& min, const int& max) {
	auto setMinMax = [&](ChunkArea* nearbyChunkArea) {
		if(nearbyChunkArea != nullptr) {
			nearbyChunkArea->minimumPoint = std::min(nearbyChunkArea->minimumPoint, (min < 0) ? 0 : min);
			nearbyChunkArea->highestPoint = std::max(nearbyChunkArea->highestPoint, max);
		}
	};

	setMinMax(chunkArea);
	setMinMax(chunkArea->nearbyChunkAreas[CHUNK_AREA_LEFT]);
	setMinMax(chunkArea->nearbyChunkAreas[CHUNK_AREA_BACK]);
	setMinMax(chunkArea->nearbyChunkAreas[CHUNK_AREA_RIGHT]);
	setMinMax(chunkArea->nearbyChunkAreas[CHUNK_AREA_FRONT]);
}

bool ChunkManager::_chunkAreaExists(const ChunkXZ& coord) {
	return (_chunkAreas.find(coord) != _chunkAreas.end());
}

void ChunkManager::_generateChunks() {
	while(true) {
		int currentChunkX = int(World::getPlayer().position.x / CHUNK_SIZE),
			currentChunkZ = int(World::getPlayer().position.z / CHUNK_SIZE);

		for(int i = 0; i <= RENDER_DISTANCE; i++)
		for(int x = currentChunkX - i; x < currentChunkX + i; x++)
		for(int z = currentChunkZ - i; z < currentChunkZ + i; z++) {
			if(World::disposed) return;
			if(x <= 0 || z <= 0) continue;

			ChunkArea* chunkArea = getChunkArea({ x, z });
			chunkArea->prepare();

			for(auto it : chunkArea->chunks) {
				if(!it.second->nearbyChunksDetected) {
					World::getChunkManager().getNearbyChunks(it.second->coord, it.second->nearbyChunks);
					it.second->nearbyChunksDetected = true;
				}

				if(!it.second->meshesGenerated)
					it.second->generateChunkMesh();
				else if(it.second->isDirty)
					it.second->recreateChunkMesh();
			}
		}
	}
}
