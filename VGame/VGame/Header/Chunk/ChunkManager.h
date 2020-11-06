#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <vector>
#include <unordered_map>
#include <map>
#include <mutex>
#include <thread>
#include <string>
#include <queue>
#include "Chunk.h"
#include "Coordinates.h"
class World;
class TerrainGenerator;
class Camera;
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
	Camera* _camera;
	std::vector<std::thread> _threads;
	// std::queue -> chunkUpdates


public:
	ChunkManager(World* world, TerrainGenerator* terrainGenerator, Camera* camera);
	~ChunkManager();

	void setPlayerSpawnPoint(Player& player);

	void getNearbyChunks(ChunkCoordXZ coord, Chunk** chunkList);
	Chunk* getChunk(ChunkCoordXZ coord);
	ChunkMap* getChunkMap(ChunkCoordXZ coord);

	bool chunkExists(ChunkCoordXZ coord);
	bool chunkMapExists(ChunkCoordXZ coord);
	std::vector<Chunk*> getChunksToRender();

	// BlockType getBlock(Chunk& chunk, int x, int y, int z);
	void placeBlock(BlockPositionXYZ blockCoord, BlockType block);

	static ChunkCoordXZ getChunkCoord(const BlockPositionXYZ& blockCoord);
	static BlockPositionXYZ getBlockCoord(const BlockPositionXYZ& blockCoord);

private:
	void _generateChunkData();

};

#endif // CHUNK_MANAGER_H
