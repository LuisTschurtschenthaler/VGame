#include "TerrainGenerator.h"
#include "Constants.h"
#include "NoiseGenerator.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include "Block.h"
#include "Biome.h"
#include "Biomes.h"
#include "Random.h"
#include "Structure.h"


TerrainGenerator::TerrainGenerator() {
	int seed = Random::get(1000, 9999);
	std::cout << "Biome-Seed: " << seed << std::endl;

	_desert = new Desert(seed);
	_grassland = new Grassland(seed);
	_forest = new Forest(seed);
	_snowForest = new SnowForest(seed);
	_highLands = new HighLands(seed);

	_biomeNoise = new NoiseGenerator(Random::get(1000, 9999));
}

TerrainGenerator::~TerrainGenerator() {
}


void TerrainGenerator::generateChunkData(const ChunkXZ& coord, Array3D<BlockID, CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE>& chunkData) {
	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++) {
		Biome* biome = _getBiome(_biomeNoise->getNoise(x, z, coord.x, coord.z));
		int height = std::ceil(biome->getHeight(x, z, coord.x, coord.z));

		for(int y = 0; y < CHUNK_HEIGHT; y++) {
			if(y > height) { // IS TYPE REALISTIC
				if(y <= WATER_LEVEL)
					chunkData.set(x, y, z, BlockID::WATER);
			}

			else if(y == height) {
				if(y >= WATER_LEVEL) {
					if(y == BEACH_LEVEL)
						chunkData.set(x, y, z, (Random::get(0, 10) <= 4) ? BlockID::SAND : biome->getTopBlock());

					else if(y == BEACH_LEVEL - 1)
						chunkData.set(x, y, z, (Random::get(0, 10) <= 8) ? BlockID::SAND : biome->getTopBlock());

					else if(y < BEACH_LEVEL)
						chunkData.set(x, y, z, BlockID::SAND);

					else chunkData.set(x, y, z, static_cast<BlockID>(biome->getTopBlock()));
				}
				else chunkData.set(x, y, z, static_cast<BlockID>(biome->getUnderwaterBlock()));
			}

			else if(y > height - 3)
				chunkData.set(x, y, z, static_cast<BlockID>(biome->getBelowTopBlock()));
			else chunkData.set(x, y, z, static_cast<BlockID>(biome->getUnderEarth()));
		}
	}
}

Biome* TerrainGenerator::_getBiome(const float& value) {
	if(value > 140) return _grassland;
	else if(value > 125) return _snowForest;
	else if(value > 110) return _highLands;
	else if(value > 100) return _forest;
	else if(value > 90) return _grassland;
	else return _desert;
}