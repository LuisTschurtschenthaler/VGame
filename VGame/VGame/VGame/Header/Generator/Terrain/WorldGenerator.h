#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include <map>
#include "Array2D.h"
#include "Array3D.h"
#include "Constants.h"
#include "BlockID.h"
#include "Coordinates.h"
#include "Biomes.h"

class NoiseGenerator;
class ChunkArea;


class WorldGenerator {

private:
	NoiseGenerator* _biomeNoise;
	ChunkArea* _chunkArea;

	Desert* _desert;
	Grassland* _grassland;
	Forest* _forest;
	BirchForest* _birchForest;
	SnowForest* _snowForest;

	Array2D<float, CHUNK_SIZE, CHUNK_SIZE> _heightMap;
	Array2D<Biome*, CHUNK_SIZE, CHUNK_SIZE> _biomeMap;


public:
	WorldGenerator();
	~WorldGenerator();

	void generateChunkArea(ChunkArea& chunkArea);

	Biome* getBiomeAt(const int& x, const int& z, const ChunkXZ& coord);
	Biome* getBiomeType(const float& value);

private:
	void _generateHeightMap();
	void _generateBiomeMap();
	void _setBlocks();
	void _generateVeins();

};

#endif // WORLD_GENERATOR_H
