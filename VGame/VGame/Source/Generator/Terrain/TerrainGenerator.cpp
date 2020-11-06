#include "TerrainGenerator.h"
#include "WorldConstants.h"
#include "NoiseGenerator.h"
#include "ChunkManager.h"
#include "Realistic.h"
#include "Flatland.h"
#include "Chunk.h"
#include "Block.h"
#include "Biome.h"
#include "Biomes.h"
#include "Random.h"
#include "Structure.h"
#include "ChunkMap.h"
#include "ChunkSection.h"


TerrainGenerator* TerrainGenerator::generators[] = {
	new Flatland(),
	new Realistic()
};


TerrainGenerator::TerrainGenerator() {
	_seed = Random::get(1000, 999999);

	_desert = new Desert(_seed);
	_grassland = new Grassland(_seed);
	_forest = new Forest(_seed);
	_snowForest = new SnowForest(_seed);
	_highLands = new HighLands(_seed);

	_biomeNoise = new NoiseGenerator(Random::get(1000, 999999));
}

TerrainGenerator::~TerrainGenerator() {
}


void TerrainGenerator::generateBlockData(ChunkSection& chunkSection, ChunkMap* chunkMap) {
	for(int x = 0; x < CHUNK_SIZE; x++) {
		for(int z = 0; z < CHUNK_SIZE; z++) {

			Biome* biome = getBiome(chunkMap, x, z);
			int height = chunkMap->heightMap.get(x, z);

			for(int y = 0; y < CHUNK_SIZE; y++) {
				int iy = y + chunkSection.coord.y * CHUNK_SIZE;

				if(iy > height) { // IS TYPE REALISTIC
					if(iy <= WATER_LEVEL)
						chunkSection.data.set(x, y, z, BlockType::WATER);
				}

				else if(iy == height) {
					if(iy >= WATER_LEVEL) {
						if(iy == BEACH_LEVEL)
							chunkSection.data.set(x, y, z, (Random::get(0, 10) <= 5) ? BlockType::SAND : static_cast<BlockType>(biome->getTopBlock()));
						else if(iy < BEACH_LEVEL)
							chunkSection.data.set(x, y, z, BlockType::SAND);
						else chunkSection.data.set(x, y, z, static_cast<BlockType>(biome->getTopBlock()));
					}
					else chunkSection.data.set(x, y, z, static_cast<BlockType>(biome->getUnderwaterBlock()));
					
				}
				
				else if(iy > height - 3)
					chunkSection.data.set(x, y, z, static_cast<BlockType>(biome->getBelowTopBlock()));

				else chunkSection.data.set(x, y, z, static_cast<BlockType>(biome->getUnderEarth()));
			}
		}
	}
}

Biome* TerrainGenerator::getBiome(ChunkMap* chunkMap, int x, int z) {
	int biome = chunkMap->biomeMap.get(x, z);

	if(biome > 140) return _grassland;
	else if(biome > 125) return _snowForest;
	else if(biome > 110) return _highLands;
	else if(biome > 100) return _forest;
	else if(biome > 90) return _grassland;
	else return _desert;
}
