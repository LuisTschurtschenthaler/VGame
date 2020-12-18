#ifndef FLATLAND_H
#define FLATLAND_H

#include "TerrainGenerator.h"
#include "WorldConstants.h"
#include "ChunkMap.h"
#include "Random.h"


class Flatland : public TerrainGenerator {

public:
	Flatland()
		: TerrainGenerator() {

		_seed = Random::get();
		std::cout << "Terrain-Seed (flatland): " << _seed << std::endl;
	}


	ChunkMap* generateChunkMap(ChunkCoordXZ coord) override {
		ChunkMap* chunkMap = new ChunkMap();
		chunkMap->max = 8;

		for(int x = 0; x < CHUNK_SIZE; x++) {
			for(int z = 0; z < CHUNK_SIZE; z++) {

				int biomeValue = _biomeNoise->getNoise(x, z, coord.x, coord.z);
				chunkMap->biomeMap.set(x, z, biomeValue);
				chunkMap->heightMap.set(x, z, 8);

			}
		}
		return chunkMap;
	}

};

#endif // FLATLAND_H
