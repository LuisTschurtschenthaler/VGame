#ifndef CHUNK_SECTION
#define CHUNK_SECTION

#include "Block.h"
#include "Coordinates.h"
#include "NearbyChunks.h"
#include "Array3D.h"
#include "AABB.h"
#include "MeshTypes.h"
class ChunkManager;
class ChunkMap;
class Chunk;


class ChunkSection {

public:
	ChunkManager* chunkManager;
	ChunkCoordXYZ coord;
	AABB aabb;
	Array3D<BlockType, CHUNK_SIZE> data;
	bool areMeshesGenerated;

private:
	Chunk* _chunk;


public:
	ChunkSection(ChunkManager* chunkManager, Chunk* chunk, const ChunkCoordXYZ& coord);
	~ChunkSection();

	void placeBlock(const BlockPositionXYZ& blockCoord, const BlockType& block);
	BlockType getBlock(const BlockPositionXYZ& blockCoord);
	void generateMesh();
	Block* getBlockRelative(int x, int y, int z);

private:
	void _addBlockFaces(int x, int y, int z, MeshType meshType, Block* block);
	bool _isOutOfChunkRange(BlockPositionXYZ coord);
	bool _isOutOfChunkRange(int nr);
	Block* _getBlock(int x, int y, int z);

};

#endif // CHUNK_SECTION
