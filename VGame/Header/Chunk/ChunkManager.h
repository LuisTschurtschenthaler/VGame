#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <unordered_map>
#include <map>
#include <mutex>
#include <vector>
#include <thread>
#include "BlockID.h"
#include "Chunk.h"
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

	void findSpawnPoint(glm::vec3& position);
	
	void getNearbyChunks(const ChunkXZ& coord, Chunk** nearbyChunks);

	void removeBlock(const LocationXYZ& location);
	void placeBlock(const LocationXYZ& location, BlockID blockID);

	Chunk* getChunk(const ChunkXZ& coord);
	Chunk* getChunkFromLocation(const LocationXYZ& location);

	LocationXYZ getBlockLocation(const LocationXYZ& location);
	BlockID getBlockID(const LocationXYZ& location);

private:
	void _setNearbyChunksDirty(Chunk* chunk, const LocationXYZ& location);
	bool _chunkExists(const ChunkXZ& coord);
	void _generateChunks();

};

#endif // CHUNK_MANAGER_H
