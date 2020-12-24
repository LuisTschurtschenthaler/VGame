#ifndef CHUNK_SECTION
#define CHUNK_SECTION

#include "Block.h"
#include "Coordinates.h"
#include "NearbyChunks.h"
#include "Array3D.h"
#include "MeshTypes.h"
#include "MeshCollection.h"

class ChunkManager;
class ChunkMap;
class Chunk;


class ChunkSection {

public:
	ChunkManager* chunkManager;
	MeshCollection* meshCollection;
	ChunkCoordXYZ coord;
	Array3D<BlockType, CHUNK_SIZE> data;
	bool areMeshesGenerated;

private:
	Chunk* _chunk;


public:
	ChunkSection(ChunkManager* chunkManager, Chunk* chunk, const ChunkCoordXYZ& coord);
	~ChunkSection();

	void placeBlock(const BlockPositionXYZ& blockCoord, const BlockType& block);
	void removeBlock(const BlockPositionXYZ& blockCoord);

	const BlockType getBlockType(const BlockPositionXYZ& blockCoord);
	const Block* getBlockRelative(int x, int y, int z) const;
	
	void generateMesh(MeshCollection* collection = nullptr);
	void recreateMeshes();

private:
	void _addBlockFaces(int x, int y, int z, MeshType meshType, Block* block, MeshCollection& collection);
	bool _isOutOfChunkRange(BlockPositionXYZ coord);
	bool _isOutOfChunkRange(int nr);
	const Block* _getBlock(int x, int y, int z) const;

};

#endif // CHUNK_SECTION
