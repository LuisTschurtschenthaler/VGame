#include <algorithm>
#include "Realistic.h"
#include "WorldConstants.h"
#include "NoiseGenerator.h"
#include "Biomes.h"
#include "Random.h"
#include "GeneralMath.h"
#include "Coordinates.h"
#include "ChunkMap.h"
#include "GeneralMath.h"


Realistic::Realistic() :
	TerrainGenerator() {

	_seed = Random::get(1000, 999999);
	std::cout << "Terrain-Seed (realistic): " << _seed << std::endl;
}


ChunkMap* Realistic::generateChunkMap(ChunkCoordXZ coord) {
	ChunkMap* chunkMap = new ChunkMap();

	_generateBiomeMap(*chunkMap, coord);
	_generateHeightMap(*chunkMap, coord);

	return chunkMap;
}


void Realistic::_generateBiomeMap(ChunkMap& chunkMap, ChunkCoordXZ coord) {
	for(int x = 0; x < CHUNK_SIZE; x++) {
		for(int z = 0; z < CHUNK_SIZE; z++) {
			int biomeValue = _biomeNoise->getHeight(x, z, coord.x, coord.z);
			chunkMap.biomeMap.set(x, z, biomeValue);
		}
	}
}

void Realistic::_generateHeightMap(ChunkMap& chunkMap, ChunkCoordXZ coord) {
	/*
	auto heightAt = [&](int x, int z) {
		Biome* biome = getBiome(&chunkMap, x, z);
		return std::ceil(biome->getHeight(x, z, coord.x, coord.z));
	};
	*/

	_getHeightIn(chunkMap, coord, 0, 0, HALF_CHUNK_SIZE, HALF_CHUNK_SIZE);
	_getHeightIn(chunkMap, coord, HALF_CHUNK_SIZE, 0, CHUNK_SIZE, HALF_CHUNK_SIZE);
	_getHeightIn(chunkMap, coord, 0, HALF_CHUNK_SIZE, HALF_CHUNK_SIZE, CHUNK_SIZE);
	_getHeightIn(chunkMap, coord, HALF_CHUNK_SIZE, HALF_CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);

	/*
	int bottomLeft	= heightAt(0, 0);
	int bottomRight = heightAt(CHUNK_SIZE_R, 0);
	int topLeft		= heightAt(0, CHUNK_SIZE_R);
	int topRight	= heightAt(CHUNK_SIZE_R, CHUNK_SIZE_R);

	for(int x = 0; x < CHUNK_SIZE; x++) {
		for(int z = 0; z < CHUNK_SIZE; z++) {
			int heightValue = heightAt(x, z);
			
			chunkMap.heightMap.set(x, z, heightValue);
			chunkMap.max = std::max(chunkMap.max, heightValue);
		}
	}
	*/
}

void Realistic::_getHeightIn(ChunkMap& chunkMap, ChunkCoordXZ coord, int xMin, int zMin, int xMax, int zMax) {
	auto heightAt = [&](int x, int z) {
		Biome* biome = getBiome(&chunkMap, x, z);
		return std::ceil(biome->getHeight(x, z, coord.x, coord.z));
	};

	int bottomLeft = heightAt(xMin, zMin);
	int bottomRight = heightAt(xMax, zMin);
	int topLeft = heightAt(xMin, zMax);
	int topRight = heightAt(xMax , zMax);

	for(int x = xMin; x < xMax; x++) {
		for(int z = zMin; z < zMax; z++) {
			int heightValue = heightAt(x, z);

			chunkMap.heightMap.set(x, z, heightValue);
			if(heightValue > chunkMap.max) chunkMap.max = heightValue;
		}
	}
}
