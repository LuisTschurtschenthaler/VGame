#include <vector>
#include <GLM/glm.hpp>
#include "WorldGenerator.h"
#include "NoiseGenerator.h"
#include "Chunk.h"
#include "Random.h"
#include "Biome.h"
#include "Structure.h"
#include "Util.h"


WorldGenerator::WorldGenerator() {
	int seed = Random::get(1000, 9999);
	std::cout << "Biome-Seed: " << seed << std::endl;

	_desert = new Desert(seed);
	_grassland = new Grassland(seed);
	_forest = new Forest(seed);
	_snowForest = new SnowForest(seed);
	_highLands = new HighLands(seed);

	_biomeNoise = new NoiseGenerator(Random::get());


	_heightMap = Array2D<float, CHUNK_SIZE, CHUNK_SIZE>();
	_biomeMap = Array2D<Biome*, CHUNK_SIZE + 1, CHUNK_SIZE + 1>();
}

WorldGenerator::~WorldGenerator() {
}


Biome* WorldGenerator::getBiomeAt(const int& x, const int& z, const ChunkXZ& coord) {
	return getBiomeType(_biomeNoise->getNoise(x, z, coord.x, coord.z));
}

void WorldGenerator::generateChunk(Chunk& chunk) {
	_chunk = &chunk;
	_biomeNoise->setSeed((chunk.coord.x ^ chunk.coord.z) << 2);

	generateBiomeMap();
	generateHeightMap();
	setBlocks();

	chunk.chunkDataGenerated = true;
}

void WorldGenerator::generateHeightMap() {
	auto getHeight = [&](int x, int z) {
		return _biomeMap.get(x, z)->getHeight(x, z, _chunk->coord.x, _chunk->coord.z);
	};

	int topLeft = getHeight(0, 0);
	int topRight = getHeight(CHUNK_SIZE, 0);
	int bottomLeft = getHeight(0, CHUNK_SIZE);
	int bottomRight = getHeight(CHUNK_SIZE, CHUNK_SIZE);

	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++) {
		_heightMap.set(x, z,
					   //glm::mix(
					   //	glm::mix(topLeft, topRight, x / (CHUNK_SIZE - 1.f)),
					   //	glm::mix(bottomLeft, bottomRight, x / (CHUNK_SIZE - 1.f)),
					   //	z / (CHUNK_SIZE - 1.f)
					   //)
					   getHeight(x, z)
		);
	}
}

void WorldGenerator::generateBiomeMap() {
	for(int x = 0; x < CHUNK_SIZE + 1; x++)
	for(int z = 0; z < CHUNK_SIZE + 1; z++)
		_biomeMap.set(x, z, getBiomeType(_biomeNoise->getNoise(x, z, _chunk->coord.x, _chunk->coord.z)));
}

void WorldGenerator::setBlocks() {
	std::vector<std::pair<Biome*, LocationXYZ>> plants, trees;

	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++) {
		Biome* biome = _biomeMap.get(x, z);
		int height = _heightMap.get(x, z);

		for(int y = 0; y < CHUNK_HEIGHT; y++) {
			if(y > height) {
				if(y <= WATER_LEVEL)
					_chunk->chunkData.set(x, y, z, BlockID::WATER);
			}

			else if(y == height) {
				if(y >= WATER_LEVEL) {
					if(y < BEACH_LEVEL) {
						_chunk->chunkData.set(x, y, z, (Random::get(0, 10) <= 4) ? BlockID::SAND : biome->getTopBlock());
						continue;
					}

					if(Random::getIntInRange(0, biome->getPlantFrequency()) == 5)
						plants.push_back(std::make_pair(biome, LocationXYZ(x, y + 1, z )));
					else if(Random::getIntInRange(0, biome->getTreeFrequency()) == 5)
						trees.push_back(std::make_pair(biome, LocationXYZ(x, y + 1, z)));

					_chunk->chunkData.set(x, y, z, biome->getTopBlock());
				}
				else _chunk->chunkData.set(x, y, z, biome->getUnderwaterBlock());
			}

			else if(y > height - 3)
				_chunk->chunkData.set(x, y, z, biome->getBelowTopBlock());
			else _chunk->chunkData.set(x, y, z, biome->getUnderEarth());
		}
	}

	for(auto& plant : plants)
		_chunk->chunkData.set(plant.second, plant.first->getPlant());

	for(auto& tree : trees) {
		Structure structure;
		if(instanceof<Desert>(tree.first))
			structure.generateCactus({ tree.second.x + _chunk->coord.x * CHUNK_SIZE, tree.second.y, tree.second.z + _chunk->coord.z * CHUNK_SIZE });
		else structure.generateTree({ tree.second.x + _chunk->coord.x * CHUNK_SIZE, tree.second.y, tree.second.z + _chunk->coord.z * CHUNK_SIZE });

		structure.build();
	}
}

Biome* WorldGenerator::getBiomeType(const float& value) {
	return _forest;

	//if(value > 160) return _ocean;
	if(value > 150) return _grassland;
	else if(value > 130) return _forest;
	else if(value > 120) return _forest;
	else if(value > 110) return _forest;
	else if(value > 100) return _grassland;
	else return _desert;
}
