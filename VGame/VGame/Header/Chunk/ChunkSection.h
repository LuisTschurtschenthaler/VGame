#ifndef CHUNK_SECTION
#define CHUNK_SECTION

#include "Block.h"
#include "Coordinates.h"
#include "NearbyChunks.h"
#include "Array3D.h"
class ChunkManager;
class ChunkMap;
class Chunk;


class ChunkSection {

public:
	ChunkManager* chunkManager;
	ChunkCoordXYZ coord;
	Array3D<BlockType, CHUNK_SIZE> data;
	bool areMeshesGenerated;

private:
	Chunk* _chunk;


public:
	ChunkSection(ChunkManager* chunkManager, Chunk* chunk, const ChunkCoordXYZ& coord);
	~ChunkSection();

	void placeBlock(BlockPositionXYZ blockCoord, const BlockType& block);
	void generateMesh();

private:
	bool _isOutOfChunkRange(BlockPositionXYZ coord);
	bool _isOutOfChunkRange(int nr);
	Block* _getBlock(int x, int y, int z);
	Block* _getBlockRelative(int x, int y, int z);

};

#endif // CHUNK_SECTION
