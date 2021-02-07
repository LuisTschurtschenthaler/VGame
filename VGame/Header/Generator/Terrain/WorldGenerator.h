#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include <map>
#include "Array2D.h"
#include "Array3D.h"
#include "Constants.h"
#include "BlockID.h"
#include "Coordinates.h"
#include "Biomes.h"

class Chunk;
class NoiseGenerator;


class WorldGenerator {

private:
	Chunk* _chunk;
	NoiseGenerator* _biomeNoise;

	Desert* _desert;
	Grassland* _grassland;
	Forest* _forest;
	SnowForest* _snowForest;
	HighLands* _highLands;

	Array2D<float, CHUNK_SIZE, CHUNK_SIZE> _heightMap;
	Array2D<Biome*, CHUNK_SIZE + 1, CHUNK_SIZE + 1> _biomeMap;


public:
	WorldGenerator();
	~WorldGenerator();

	Biome* getBiomeAt(const int& x, const int& z, const ChunkXZ& coord);

	void generateChunk(Chunk& chunk);
	void generateHeightMap();

	void generateBiomeMap();

	void setBlocks();

	Biome* getBiomeType(const float& value);

};

#endif // WORLD_GENERATOR_H
