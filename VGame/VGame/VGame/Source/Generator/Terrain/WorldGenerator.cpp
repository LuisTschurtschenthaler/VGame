#include <vector>
#include <GLM/glm.hpp>
#include "WorldGenerator.h"
#include "NoiseGenerator.h"
#include "ChunkArea.h"
#include "Random.h"
#include "Biome.h"
#include "Block.h"
#include "Vein.h"
#include "World.h"
#include "ChunkManager.h"


WorldGenerator::WorldGenerator() {
	int seed = Random::get(100, 9999);
	std::cout << "Biome-Seed: " << seed << std::endl;

	_desert = new Desert(seed);
	_grassland = new Grassland(seed);
	_forest = new Forest(seed);
	_birchForest = new BirchForest(seed);
	_snowForest = new SnowForest(seed);

	_biomeNoise = new NoiseGenerator(Random::get(100, 9999));

	_heightMap = Array2D<float, CHUNK_SIZE, CHUNK_SIZE>();
	_biomeMap = Array2D<Biome*, CHUNK_SIZE, CHUNK_SIZE>();
}

WorldGenerator::~WorldGenerator() {
	delete _biomeNoise;

	delete _snowForest;
	delete _birchForest;
	delete _forest;
	delete _grassland;
	delete _desert;
}

void WorldGenerator::generateChunkArea(ChunkArea& chunkArea) {
	_chunkArea = &chunkArea;

	_generateBiomeMap();
	_generateHeightMap();
	_setBlocks();
	// Caves
	//_generateVeins();
	_chunkArea->chunkDataGenerated = true;
}

Biome* WorldGenerator::getBiomeAt(const int& x, const int& z, const ChunkXZ& coord) {
	return getBiomeType(_biomeNoise->getNoise(x, z, coord.x, coord.z));
}

Biome* WorldGenerator::getBiomeType(const float& value) {
	if(value > 150) return _snowForest;
	else if(value > 140) return _forest;
	else if(value > 130) return _birchForest;
	else if(value > 120) return _grassland;
	else return _desert;
}

void WorldGenerator::_generateHeightMap() {
	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++) {
		const int heightValue = _biomeMap.get(x, z)->getHeight(x, z, _chunkArea->coord.x, _chunkArea->coord.z);

		_chunkArea->minimumPoint = std::min(_chunkArea->minimumPoint, heightValue);
		_chunkArea->highestPoint = std::max(_chunkArea->highestPoint, heightValue + 10);
		_heightMap.set(x, z, heightValue);
	}
}

void WorldGenerator::_generateBiomeMap() {
	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++)
		_biomeMap.set(x, z, getBiomeType(_biomeNoise->getNoise(x, z, _chunkArea->coord.x, _chunkArea->coord.z)));
}

void WorldGenerator::_setBlocks() {
	int maxChunk = static_cast<int>(_chunkArea->highestPoint / CHUNK_SIZE);
	
	for(int c = 0; c <= (maxChunk + 1); c++) {
		Chunk* chunk = _chunkArea->getChunk(c);

		for(int x = 0; x < CHUNK_SIZE; x++)
		for(int y = 0; y < CHUNK_SIZE; y++)
		for(int z = 0; z < CHUNK_SIZE; z++) {
			Biome* biome = _biomeMap.get(x, z);
			int height = _heightMap.get(x, z);

			int actualY = y + c * CHUNK_SIZE;
			if(actualY > height) {
				if(actualY <= WATER_LEVEL)
					chunk->chunkData.set(x, y, z, BlockID::WATER);
			}

			else if(actualY == height) {
				if(actualY >= WATER_LEVEL) {
					if(actualY < BEACH_LEVEL) {
						if(actualY == BEACH_LEVEL - 3)
							chunk->chunkData.set(x, y, z, BlockID::SAND);
						else if(actualY == BEACH_LEVEL - 2)
							chunk->chunkData.set(x, y, z, (Random::get(0, 10) <= 8) ? BlockID::SAND : biome->getTopBlock());
						else if(actualY == BEACH_LEVEL - 1)
							chunk->chunkData.set(x, y, z, (Random::get(0, 10) <= 4) ? BlockID::SAND : biome->getTopBlock());
						continue;
					}

					LocationXYZ loc = { x + chunk->coord.x * CHUNK_SIZE,
										y + chunk->coord.y * CHUNK_SIZE + 1,
										z + chunk->coord.z * CHUNK_SIZE };

					if(Random::getIntInRange(0, biome->getPlantFrequency()) == 5)
						_chunkArea->_plants.push_back(std::make_pair(biome, loc));
					else if(Random::getIntInRange(0, biome->getTreeFrequency()) == 5)
						_chunkArea->_trees.push_back(std::make_pair(biome, loc));

					chunk->chunkData.set(x, y, z, biome->getTopBlock());
				}
				else chunk->chunkData.set(x, y, z, biome->getUnderwaterBlock());
			}

			else if(actualY > height - 3)
				chunk->chunkData.set(x, y, z, biome->getBelowTopBlock());
			
			else if(actualY == 0)
				chunk->chunkData.set(x, y, z, BlockID::BEDROCK);
			else if(actualY == 1 && Random::getIntInRange(0, 10) <= 9)
				chunk->chunkData.set(x, y, z, BlockID::BEDROCK);
			else if(actualY == 2 && Random::getIntInRange(0, 10) <= 6)
				chunk->chunkData.set(x, y, z, BlockID::BEDROCK);
			else if(actualY == 3 && Random::getIntInRange(0, 10) <= 3)
				chunk->chunkData.set(x, y, z, BlockID::BEDROCK);
			else chunk->chunkData.set(x, y, z, biome->getUnderEarth());
		}
	}
}

void WorldGenerator::_generateVeins() {
	Vein::generate(_chunkArea, { BlockID::COAL_ORE,		20,    4, 17,   0, 128 });
	Vein::generate(_chunkArea, { BlockID::IRON_ORE,		20,    2,  9,   0,  64 });
	Vein::generate(_chunkArea, { BlockID::GOLD_ORE,		 2,    2,  9,   0,  32 });
	Vein::generate(_chunkArea, { BlockID::LAPIS_ORE,	 2,    2,  7,   0,  32 });
	Vein::generate(_chunkArea, { BlockID::EMERALD_ORE,	 1,    1,  2,   8,  32 });
	Vein::generate(_chunkArea, { BlockID::DIAMOND_ORE,   2,    2,  8,   0,  16 });
															   		 
	Vein::generate(_chunkArea, { BlockID::DIRT,			10,   10, 33,   0, 256 });
	Vein::generate(_chunkArea, { BlockID::GRAVEL,		 8,   10, 33,   0, 256 });
}
