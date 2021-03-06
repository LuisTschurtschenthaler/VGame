#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <unordered_map>
#include <mutex>
#include <vector>
#include <thread>
#include "Block.h"
#include "Chunk.h"
#include "Entity.h"
#include "Coordinates.h"

class TextureAtlas;
class Shader;


class ChunkManager {

public:
	std::mutex chunkMutex;
	std::unordered_map<ChunkXZ, Chunk*> _chunks;

private:
	TextureAtlas* _textureAtlas;
	Shader* _solidShader, *_waterShader;
	std::vector<std::thread> _threads;


public:
	ChunkManager();
	~ChunkManager();

	void update();
	void draw();

	void findSpawnPoint(Entity& entity);
	void getNearbyChunks(const ChunkXZ& coord, Chunk** nearbyChunks);

	void placeBlock(const LocationXYZ& location, const BlockID& blockID);
	void replaceBlock(const LocationXYZ& location, const BlockID& blockToReplace, const BlockID& block);
	void removeBlock(const LocationXYZ& location);

	Chunk* getChunk(const ChunkXZ& coord);
	Chunk* getChunkFromLocation(const LocationXYZ& location);

	const LocationXYZ getBlockLocation(const LocationXYZ& location) const;
	BlockID getBlockID(const LocationXYZ& location);

	const int getTextureAtlasID() const { return _textureAtlas->getTextureID(); }
	const int getAmountOfLoadedChunks() const { return _chunks.size(); }

	static const bool isLocationOutOfChunkRange(const LocationXYZ& location);

private:
	std::vector<Chunk*> _getSortedCunks(const int& playerX, const int& playerZ);
	void _setNearbyChunksDirty(Chunk* chunk, const LocationXYZ& location);
	void _setNearbyChunksMinMax(Chunk* chunk, const int& min, const int& max);
	bool _chunkExists(const ChunkXZ& coord);
	
	void _generateChunks();

};

#endif // CHUNK_MANAGER_H
