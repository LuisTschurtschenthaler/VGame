#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <GLM/glm.hpp>
#include "Coordinates.h"
#include "Biomes.h"
class Chunk;
class ChunkMap;
class ChunkSection;
class ChunkManager;
class NoiseGenerator;
class Biome;


enum TerrainGeneratorType {
	FLATLAND,
	REALISTIC
};


class TerrainGenerator {

private:
	int _seed;

public:
	static TerrainGenerator* generators[];

protected:
	NoiseGenerator* _biomeNoise;
	int _seed;

private:
	Desert* _desert;
	Grassland* _grassland;
	Forest* _forest;
	SnowForest* _snowForest;
	HighLands* _highLands;


public:
	TerrainGenerator();
	~TerrainGenerator();
	
	void generateBlockData(ChunkSection& chunkSection, ChunkMap* chunkMap);
	Biome* getBiome(ChunkMap* chunkMap, int x, int z);
	
	//static void generateFlora(Chunk& chunk, ChunkMap* chunkMap);
	virtual ChunkMap* generateChunkMap(ChunkCoordXZ coord) = 0;

};

#endif // TERRAIN_GENERATOR_H
