#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "Block.h"
#include "Coordinates.h"
#include "MeshTypes.h"
#include "NearbyChunks.h"
#include "ChunkMesh.h"
class ChunkManager;
class ChunkSection;
class ChunkMap;


class Chunk {

public:
	ChunkManager* chunkManager;
	ChunkCoordXZ coord;
	Chunk* nearbyChunks[AMOUNT_OF_NEARBY_CHUNKS];

	std::vector<ChunkMesh*> meshCollection;
	bool chunkDataGenerated, meshGenerated, isBuffered;
	
private:
	std::vector<ChunkSection*> _sections;

public:
	Chunk(ChunkManager* chunkManager, ChunkCoordXZ coord);
	~Chunk();

	ChunkSection* getChunkSection(int y);
	void placeBlock(const BlockPositionXYZ& coord, BlockType block);

	void generateChunkData(ChunkMap* chunkMap);
	void generateFlora(ChunkMap* chunkMap);
	void generateMesh();

	void prepareDraw(int meshtype);
	void draw(int meshtype);
	void save();

	int getWorldPositionX(int x) const;
	int getWorldPositionZ(int z) const;
	
private:
	float _vertexAO(bool side1, bool side2, bool corner);

};

#endif // CHUNK_H
