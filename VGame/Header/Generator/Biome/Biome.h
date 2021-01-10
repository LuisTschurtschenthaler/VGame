#ifndef BIOME_H
#define BIOME_H

#include <string>
#include "NoiseGenerator.h"
#include "Constants.h"
#include "BlockID.h"


enum BiomeType {
	OCEAN = 0,
	DESERT,
	GRASSLAND,
	FOREST,
	SNOW_FOREST,
	HIGH_LANDS,

	TOTAL_BIOMES
};


class Biome {

private:
	NoiseGenerator* _noise;
	int _treeFrequency, _plantFrequency;


public:
	Biome(int treeFrequency, int plantFrequency, int seed);
	~Biome();

	int getHeight(int x, int z, int chunkX, int chunkZ);

	virtual std::string getName() const = 0;
	virtual BlockID getPlant() const = 0;
	virtual BlockID getTopBlock() const = 0;
	virtual BlockID getBelowTopBlock() const = 0;
	virtual BlockID getUnderEarth() const = 0;
	virtual BlockID getUnderwaterBlock() const = 0;

	int getTreeFrequency() const { return _treeFrequency; }
	int getPlantFrequency() const { return _plantFrequency; }
};

#endif // BIOME_H
