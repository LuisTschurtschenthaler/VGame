#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <cstdlib>
#include <ctime>
#include <GLM/glm.hpp>
#include "Coordinates.h"
#include "Constants.h"
#include "Biomes.h"
#include "Array3D.h"

class Chunk;
class ChunkMap;
class ChunkManager;
class NoiseGenerator;
class Biome;


enum TerrainGeneratorType {
	FLATLAND,
	REALISTIC
};


class TerrainGenerator {

	friend class Chunk;

private:
	NoiseGenerator* _biomeNoise;

	Desert* _desert;
	Grassland* _grassland;
	Forest* _forest;
	SnowForest* _snowForest;
	HighLands* _highLands;


public:
	TerrainGenerator();
	~TerrainGenerator();
	
	void generateChunkData(const ChunkXZ& coord, Array3D<BlockID, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE>& chunkData);
	//static void generateFlora(Chunk& chunk, ChunkMap* chunkMap);

private:
	Biome* _getBiome(const float& value);

};

#endif // TERRAIN_GENERATOR_H
