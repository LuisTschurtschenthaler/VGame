#include "ChunkManager.h"
#include "Camera.h"
#include "World.h"
#include "WorldConstants.h"
#include "TerrainGenerator.h"
#include "NearbyChunks.h"
#include "ChunkMesh.h"
#include "Player.h"
#include "Random.h"
#include "Chunk.h"
#include "ChunkMap.h"
#include "ChunkSection.h"
#include "FileManager.h"
#include "Structure.h"


ChunkManager::ChunkManager(World* world, TerrainGenerator* terrainGenerator, Camera* camera)
	: terrainGenerator(terrainGenerator), _world(world), _camera(camera) {

	//for(int i = 0; i < 4; i++)
		_threads.emplace_back([&]() {
			while(!_world->disposed) {
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				_generateChunkData();
			}
	});
}

ChunkManager::~ChunkManager() {
	for(auto& thread : _threads)
		thread.join();

	chunkMapMutex.lock();
	for(auto& it : chunks)
		it.second->~Chunk();
	chunkMapMutex.unlock();
}


void ChunkManager::setPlayerSpawnPoint(Player& player) {
	int posX = Random::get(0, CHUNK_SIZE - 1),
		posZ = Random::get(0, CHUNK_SIZE - 1);

	ChunkMap* map = getChunkMap({ 0, 0 });
	int heightValue = map->heightMap.get(posX, posZ) + 3;
	int height = (heightValue > WATER_LEVEL) ? heightValue : WATER_LEVEL + 2;

	player.setSpawnPoint({ posX + 0.5f, height + 10, posZ + 0.5f });
}

/*
std::vector<BlockPositionXYZ>* ChunkManager::getAdjacentBlocks(glm::vec3 blockCoord) {
	std::vector<BlockPositionXYZ>* adjacentBlocks = new std::vector<BlockPositionXYZ>();
	std::vector<glm::vec3> adjacents = {
		{ -1,  0,  0 },
		{  1,  0,  0 },
		{  0, -1,  0 },
		{  0,  1,  0 },
		{  0,  0, -1 },
		{  0,  0,  1 }
	};

	for(auto& adjacent : adjacents) {
		blockCoord = blockCoord + adjacent;
		int x = int(std::floor(blockCoord.x));
		int y = int(std::floor(blockCoord.y));
		int z = int(std::floor(blockCoord.z));

		adjacentBlocks->push_back({ x, y, z });
	}

	return adjacentBlocks;
}*/

void ChunkManager::getNearbyChunks(const ChunkCoordXZ& coord, Chunk** chunkList) {
	chunkList[CHUNK_RIGHT]	 = getChunk({ coord.x + 1, coord.z });
	chunkList[CHUNK_LEFT]	 = getChunk({ coord.x - 1, coord.z });
	chunkList[CHUNK_FRONT]	 = getChunk({ coord.x, coord.z + 1 });
	chunkList[CHUNK_BACK]	 = getChunk({ coord.x, coord.z - 1 });
}

Chunk* ChunkManager::getChunk(const ChunkCoordXZ& coord) {
	chunkMapMutex.lock();
	if(!chunkExists(coord))
		chunks.emplace(coord, new Chunk(this, coord));

	chunkMapMutex.unlock();
	return chunks[coord];
}

ChunkMap* ChunkManager::getChunkMap(const ChunkCoordXZ& coord) {
	if(!chunkMapExists(coord))
		chunkMaps.emplace(coord, terrainGenerator->generateChunkMap(coord));
	
	return chunkMaps[coord];
}

bool ChunkManager::chunkExists(const ChunkCoordXZ& coord) {
	return (chunks.find(coord) != chunks.end());
}

bool ChunkManager::chunkMapExists(const ChunkCoordXZ& coord) {
	return (chunkMaps.find(coord) != chunkMaps.end());
}

std::vector<Chunk*> ChunkManager::getChunksToRender() {
	std::vector<Chunk*> chunksToRender = std::vector<Chunk*>();
	
	int playerX = static_cast<int>(_camera->getPosition().x / CHUNK_SIZE);
	int playerZ = static_cast<int>(_camera->getPosition().z / CHUNK_SIZE);

	for(std::pair<ChunkCoordXZ, Chunk*> it : chunks) {
		if(!it.second->meshGenerated)
			continue;

		if(it.first.x < playerX - RENDER_DISTANCE || it.first.x > playerX + RENDER_DISTANCE ||
		   it.first.z < playerZ - RENDER_DISTANCE || it.first.z > playerZ + RENDER_DISTANCE) {

			/* Unload chunk and save to file */
			//FileManager::save(*it.second);

		}
		else chunksToRender.push_back(it.second);

	}
	return chunksToRender;
}

BlockType ChunkManager::getBlock(const BlockPositionXYZ& coord) {
	ChunkCoordXZ chunkCoord = getChunkCoord(coord);
	BlockPositionXYZ bCoord = getBlockCoord(coord);

	ChunkSection* section = getChunk(chunkCoord)->getChunkSection(coord.y / CHUNK_SIZE);
	if(section == nullptr)
		return BlockType::AIR;
	else return section->getBlock(bCoord);
}

void ChunkManager::placeBlock(BlockPositionXYZ blockCoord, BlockType block) {
	ChunkCoordXZ chunkCoord = getChunkCoord(blockCoord);
	getChunk(chunkCoord)->placeBlock(blockCoord, block);
}

ChunkCoordXZ ChunkManager::getChunkCoord(const BlockPositionXYZ& blockCoord) {
	ChunkCoordXZ c{ blockCoord.x / CHUNK_SIZE, blockCoord.z / CHUNK_SIZE };
	if(c.x < 0) c.x -= 1;
	if(c.z < 0) c.z -= 1;
	
	return c;
}

BlockPositionXYZ ChunkManager::getBlockCoord(const BlockPositionXYZ& blockCoord) {
	BlockPositionXYZ c{ blockCoord.x % CHUNK_SIZE, blockCoord.y % CHUNK_SIZE, blockCoord.z % CHUNK_SIZE };
	if(c.x < 0) c.x += CHUNK_SIZE;
	if(c.y < 0) c.y += CHUNK_SIZE;
	if(c.z < 0) c.z += CHUNK_SIZE;

	return c;
}


void ChunkManager::_generateChunkData() {
	int currentChunkX = static_cast<int>(_camera->getPosition().x / CHUNK_SIZE);
	int currentChunkZ = static_cast<int>(_camera->getPosition().z / CHUNK_SIZE);

	for(int i = 0; i < RENDER_DISTANCE; i++) {
		for(int x = currentChunkX - i; x <= currentChunkX + i; x++) {
			for(int z = currentChunkZ - i; z <= currentChunkZ + i; z++) {
				if(x <= 0 || z <= 0) continue;

				int chunkX = currentChunkX + x;
				int chunkZ = currentChunkZ + z;

				Chunk* chunk = getChunk({ x, z });
				if(!chunk->chunkDataGenerated)
					chunk->generateChunkData(getChunkMap({ x, z }));

				if(!chunk->meshGenerated && chunk->chunkDataGenerated)
					chunk->generateMesh();
			}
		}
	}
}
