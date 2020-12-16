#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <vector>
#include <unordered_map>
#include <map>
#include <mutex>
#include <thread>
#include <string>
#include <queue>
#include <GLM/glm.hpp>
#include "Chunk.h"
#include "Coordinates.h"
class World;
class TerrainGenerator;
class Player;
class ChunkMap;


class ChunkManager {

public:
	TerrainGenerator* terrainGenerator;
	std::unordered_map<ChunkCoordXZ, Chunk*> chunks;
	std::unordered_map<ChunkCoordXZ, ChunkMap*> chunkMaps;
	std::mutex chunkMapMutex;

private:
	World* _world;
	Player* _player;
	std::vector<std::thread> _threads;


public:
	ChunkManager(World* world, TerrainGenerator* terrainGenerator, Player* player);
	~ChunkManager();

	void setPlayerSpawnPoint(Player& player);

	void getNearbyChunks(const ChunkCoordXZ& coord, Chunk** chunkList);
	Chunk* getChunk(const ChunkCoordXZ& coord);
	ChunkMap* getChunkMap(const ChunkCoordXZ& coord);

	bool chunkExists(const ChunkCoordXZ& coord);
	bool chunkMapExists(const ChunkCoordXZ& coord);
	std::vector<Chunk*> getChunksToRender();

	BlockType getBlock(const BlockPositionXYZ& coord);
	void placeBlock(BlockPositionXYZ blockCoord, BlockType block);

	static ChunkCoordXZ getChunkCoord(const BlockPositionXYZ& blockCoord);
	static BlockPositionXYZ getBlockCoord(const BlockPositionXYZ& blockCoord);

	World* getWorld() const { return _world; }

private:
	void _generateChunkData();

};

#endif // CHUNK_MANAGER_H