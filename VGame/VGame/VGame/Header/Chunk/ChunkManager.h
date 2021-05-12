#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <unordered_map>
#include <mutex>
#include <vector>
#include <thread>
#include "Block.h"
#include "Chunk.h"
#include "ChunkArea.h"
#include "Entity.h"
#include "Coordinates.h"

class TextureAtlas;
class Shader;


class ChunkManager {

public:
	std::mutex chunkAreaMutex;
	std::unordered_map<ChunkXZ, ChunkArea*> _chunkAreas;

private:
	TextureAtlas* _textureAtlas;
	Shader* _chunkShader;
	std::vector<std::thread> _threads;


public:
	ChunkManager();
	~ChunkManager();

	void update();
	void draw();

	void findSpawnPoint(Entity& entity);

	void getNearbyChunks(const ChunkXYZ& coord, Chunk** nearbyChunks);
	void getNearbyChunkAreas(const ChunkXZ& coord, ChunkArea** nearbyChunks);

	void placeBlock(const LocationXYZ& location, const BlockID& blockID);
	void replaceBlock(const LocationXYZ& location, const BlockID& blockToReplace, const BlockID& block);
	void removeBlock(const LocationXYZ& location);

	ChunkArea* getChunkArea(const ChunkXZ& coord);

	Chunk* getChunk(const ChunkXYZ& coord);
	Chunk* getChunkFromLocation(const LocationXYZ& location);

	LocationXYZ getBlockLocation(const LocationXYZ& location);
	BlockID getBlockID(const LocationXYZ& location);

	const int getAmountOfLoadedChunks();
	static const bool isLocationOutOfChunkRange(const LocationXYZ& location);

private:
	std::vector<Chunk*> _getSortedCunks(const int& playerX, const int& playerY, const int& playerZ);
	void _setNearbyChunksDirty(Chunk* chunk, const LocationXYZ& location);
	void _setNearbyChunkAreasMinMax(ChunkArea* chunkArea, const int& min, const int& max);
	bool _chunkAreaExists(const ChunkXZ& coord);
	void _generateChunks();

};

#endif // CHUNK_MANAGER_H
