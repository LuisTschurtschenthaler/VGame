#include <algorithm>
#include "Realistic.h"
#include "WorldConstants.h"
#include "NoiseGenerator.h"
#include "Biomes.h"
#include "Random.h"
#include "GeneralMath.h"
#include "Coordinates.h"
#include "ChunkMap.h"


Realistic::Realistic() :
	TerrainGenerator() {

	_seed = Random::get(1000, 999999);
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
	auto heightAt = [&](int x, int z) {
		Biome* biome = getBiome(&chunkMap, x, z);
		return std::ceil(biome->getHeight(x, z, coord.x, coord.z));
	};

	int bottomLeft	= heightAt(0, 0);
	int bottomRight = heightAt(CHUNK_SIZE, 0);
	int topLeft		= heightAt(0, CHUNK_SIZE);
	int topRight	= heightAt(CHUNK_SIZE, CHUNK_SIZE);

	for(int x = 0; x < CHUNK_SIZE; x++) {
		for(int z = 0; z < CHUNK_SIZE; z++) {
			Biome* biome = getBiome(&chunkMap, x, z);
			int heightValue = static_cast<int>(std::ceil(biome->getHeight(x, z, coord.x, coord.z)));

			chunkMap.heightMap.set(x, z, heightValue);
			chunkMap.max = std::max(chunkMap.max, heightValue);
		}
	}
}
