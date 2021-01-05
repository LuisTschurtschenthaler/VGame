#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <unordered_map>
#include <map>
#include <mutex>
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
	std::thread* _chunkGenerationThread;


public:
	ChunkManager();
	~ChunkManager();

	void update();
	void draw();

	void getNearbyChunks(const ChunkXZ& coord, Chunk** nearbyChunks);
	void removeBlock(const LocationXYZ& location);
	void placeBlock(const LocationXYZ& location, BlockID blockID);

	Chunk* getChunk(const ChunkXZ& coord);

	LocationXYZ getBlockLocation(LocationXYZ location);
	BlockID getBlockID(const LocationXYZ& loc);
	bool _chunkExists(const ChunkXZ& coord);

private:
	void _generateChunks();

};

#endif // CHUNK_MANAGER_H
