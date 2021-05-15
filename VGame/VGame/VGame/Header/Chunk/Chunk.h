#ifndef CHUNK_H
#define CHUNK_H

#include <GLM/glm.hpp>
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

class AABB;
class WorldGeneration;


class Chunk {

public:
	const ChunkXYZ coord, worldCoord;
	Chunk* nearbyChunks[TOTAL_NEARBY_CHUNKS];

	Array3D<BlockID, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE> chunkData;
	bool meshesGenerated, nearbyChunksDetected, isDirty;

private:
	ChunkMesh* _solid, *_fluid;
	AABB* _aabb;


public:
	Chunk(const ChunkXYZ& coord);
	~Chunk();

	void drawSolid() const;
	void drawFluid() const;

	void generateChunkMesh(ChunkMesh* solid = nullptr, ChunkMesh* fluid = nullptr);
	void recreateChunkMesh();

	const Block& getBlockRelative(const LocationXYZ& location) const;
	const Block& getBlockRelative(const int& x, const int& y, const int& z) const;

private:
	const Block& _getBlock(const LocationXYZ& location) const;
	const Block& _getBlock(const int& x, const int& y, const int& z) const;

};

#endif // CHUNK_H
