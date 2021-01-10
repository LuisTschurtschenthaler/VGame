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
	
	Biome* getBiomeAt(const int& x, const int& z, const ChunkXZ& chunkCoord);

	void generateChunkData(const ChunkXZ& coord, Array3D<BlockID, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE>& chunkData);
	void generateFlora(const ChunkXZ& coord, Array3D<BlockID, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE>& chunkData);
	void generateCaves(const ChunkXZ& coord, Array3D<BlockID, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE>& chunkData);
	void generateOres(const ChunkXZ& coord, Array3D<BlockID, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE>& chunkData);

private:
	Biome* _getBiome(const float& value);

};

#endif // TERRAIN_GENERATOR_H
