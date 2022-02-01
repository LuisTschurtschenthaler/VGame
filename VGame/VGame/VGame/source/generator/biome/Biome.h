#ifndef BIOME_H
#define BIOME_H

#include <string>
#include "BlockID.h"

class NoiseGenerator;


enum BiomeType {
	OCEAN = 0,
	DESERT,
	GRASSLAND,
	FOREST,
	SNOW_FOREST,

	TOTAL_BIOMES
};


class Biome {

private:
	NoiseGenerator* _noise;
	int _treeFrequency, _plantFrequency;


public:
	Biome(const int& treeFrequency, const int& plantFrequency, const int& seed);
	~Biome();


	int getHeight(const int& x, const int& z, const int& chunkX, const int& chunkZ);

	virtual std::string getName() const = 0;
	virtual BlockID getPlant() const = 0;
	virtual BlockID getTopBlock() const = 0;
	virtual BlockID getBelowTopBlock() const = 0;
	virtual BlockID getUnderEarth() const = 0;
	virtual BlockID getUnderwaterBlock() const = 0;

	const int& getTreeFrequency() const { return _treeFrequency; }
	const int& getPlantFrequency() const { return _plantFrequency; }
};

#endif // BIOME_H
