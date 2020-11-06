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
	int chunkX = Random::get(0, 20), chunkZ = Random::get(0, 20);
	int posX = 8, posZ = 8;

	ChunkMap* map = getChunkMap({ chunkX, chunkZ });
	int heightValue = map->heightMap.get(posX, posZ);
	int height = (heightValue > WATER_LEVEL) ? heightValue : WATER_LEVEL + 1;

	player.setSpawnPoint({ 8.5f, height, 8.5f });
}

void ChunkManager::getNearbyChunks(ChunkCoordXZ coord, Chunk** chunkList) {
	chunkList[CHUNK_RIGHT]	 = getChunk({ coord.x + 1, coord.z });
	chunkList[CHUNK_LEFT]	 = getChunk({ coord.x - 1, coord.z });
	chunkList[CHUNK_FRONT]	 = getChunk({ coord.x, coord.z + 1 });
	chunkList[CHUNK_BACK]	 = getChunk({ coord.x, coord.z - 1 });
}

Chunk* ChunkManager::getChunk(ChunkCoordXZ coord) {
	if(!chunkExists(coord))
		chunks.emplace(coord, new Chunk(this, coord));
	
	return chunks[coord];
}

ChunkMap* ChunkManager::getChunkMap(ChunkCoordXZ coord) {
	if(!chunkMapExists(coord))
		chunkMaps.emplace(coord, terrainGenerator->generateChunkMap(coord));
	
	return chunkMaps[coord];
}

bool ChunkManager::chunkExists(ChunkCoordXZ coord) {
	return (chunks.find(coord) != chunks.end());
}

bool ChunkManager::chunkMapExists(ChunkCoordXZ coord) {
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

void ChunkManager::placeBlock(BlockPositionXYZ blockCoord, BlockType block) {
	ChunkCoordXZ chunkCoord = getChunkCoord(blockCoord);
	//BlockPositionXYZ bCoord = getBlockCoord(blockCoord);

	getChunk(chunkCoord)->placeBlock(blockCoord, block);
}


ChunkCoordXZ ChunkManager::getChunkCoord(const BlockPositionXYZ& blockCoord) {
	ChunkCoordXZ coord{ blockCoord.x / CHUNK_SIZE, blockCoord.z / CHUNK_SIZE };
	if(blockCoord.x < 0) coord.x -= 1;
	if(blockCoord.z < 0) coord.z -= 1;

	return coord;
}

BlockPositionXYZ ChunkManager::getBlockCoord(const BlockPositionXYZ& blockCoord) {
	BlockPositionXYZ coord{ blockCoord.x % CHUNK_SIZE, blockCoord.y % CHUNK_SIZE, blockCoord.z % CHUNK_SIZE };
	if(blockCoord.x < 0) coord.x += CHUNK_SIZE;
	if(blockCoord.y < 0) coord.y += CHUNK_SIZE;
	if(blockCoord.z < 0) coord.z += CHUNK_SIZE;

	return coord;
}


void ChunkManager::_generateChunkData() {
	int xi = static_cast<int>(_camera->getPosition().x / CHUNK_SIZE);
	int zi = static_cast<int>(_camera->getPosition().z / CHUNK_SIZE);

	for(int i = -RENDER_DISTANCE; i < RENDER_DISTANCE; i++) {
		for(int j = -RENDER_DISTANCE; j < RENDER_DISTANCE; j++) {
			int x = xi + i;
			int z = zi + j;

			//if((_camera->getPosition().x + RENDER_DISTANCE) > x || (_camera->getPosition().x - RENDER_DISTANCE) < x
			//   || (_camera->getPosition().z + RENDER_DISTANCE) > z || (_camera->getPosition().z - RENDER_DISTANCE) < z)
			//	return;

			Chunk* chunk = getChunk({ x, z });
			if(!chunk->chunkDataGenerated)
				chunk->generateChunkData(getChunkMap({ x, z }));

			if(!chunk->meshGenerated && chunk->chunkDataGenerated)
				chunk->generateMesh();
		}
	}
}
