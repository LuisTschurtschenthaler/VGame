#ifndef REALISTIC_H
#define REALISTIC_H

#include "TerrainGenerator.h"
class ChunkMap;

class Realistic : public TerrainGenerator {

public:
	Realistic();

	ChunkMap* generateChunkMap(ChunkCoordXZ coord) override;

private:
	void _generateBiomeMap(ChunkMap& chunkMap, ChunkCoordXZ coord);
	void _generateHeightMap(ChunkMap& chunkMap, ChunkCoordXZ coord);
	void _getHeightIn(ChunkMap& chunkMap, ChunkCoordXZ coord, int xMin, int zMin, int xMax, int zMax);

};

#endif // REALISTIC_H
