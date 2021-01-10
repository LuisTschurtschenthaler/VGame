#ifndef CHUNK_H
#define CHUNK_H

#include <GLEW/GL/glew.h>
#include <vector>
#include "Block.h"
#include "World.h"
#include "Coordinates.h"
#include "MeshTypes.h"
#include "Orientation.h"
#include "ChunkMesh.h"
#include "Constants.h"
#include "Array3D.h"

class ChunkManager;
class ChunkMap;


class Chunk {

public:
	ChunkManager* chunkManager;
	const ChunkXZ coord, worldCoord;

	Array3D<BlockID, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE> chunkData;
	bool chunkDataGenerated, meshesGenerated;
	
private:
	ChunkMesh* solid, *fluid;
	Chunk* _nearbyChunks[TOTAL_NEARBY_CHUNKS];


public:
	Chunk(ChunkManager* chunkManager, const ChunkXZ& coord);
	~Chunk();

	void drawSolid();
	void drawFluid();

	void generateChunkData();
	void generateChunkMesh();

	const glm::mat4& getModel();

	const Block* getBlockRelative(const LocationXYZ& location) const;
	const Block* getBlockRelative(const int& x, const int& y, const int& z) const;

private:
	const Block* _getBlock(const LocationXYZ& location) const;
	const Block* _getBlock(const int& x, const int& y, const int& z) const;

};

#endif // CHUNK_H
