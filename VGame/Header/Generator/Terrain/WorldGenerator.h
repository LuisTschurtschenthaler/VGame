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
class Chunk;


class WorldGenerator {

private:
	NoiseGenerator* _biomeNoise;
	Chunk* _chunk;

	Desert* _desert;
	Grassland* _grassland;
	Forest* _forest;
	BirchForest* _birchForest;
	JungleForest* _jungleForest;

	Array2D<float, CHUNK_SIZE, CHUNK_SIZE> _heightMap;
	Array2D<Biome*, CHUNK_SIZE + 1, CHUNK_SIZE + 1> _biomeMap;
	//Array3D<float, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE> _heightMap3D;


public:
	WorldGenerator();
	~WorldGenerator();

	void generateChunk(Chunk& chunk);
	void generateHeightMap();
	void generateBiomeMap();
	void setBlocks();

	Biome* getBiomeAt(const int& x, const int& z, const ChunkXZ& coord);
	Biome* getBiomeType(const float& value);

};

#endif // WORLD_GENERATOR_H
