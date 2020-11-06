#ifndef BIOME_H
#define BIOME_H

#include <string>
#include "NoiseGenerator.h"
#include "BlockType.h"

class ChunkSection;

class Biome {

private:
	NoiseGenerator* _noise;
	int _treeFrequency, _plantFrequency;

public:
	Biome(int treeFrequency, int plantFrequency, int seed);
	~Biome();

	int getHeight(int x, int z, int chunkX, int chunkZ);

	virtual std::string getName() const = 0;
	virtual BlockType getPlant() const = 0;
	virtual BlockType getTopBlock() const = 0;
	virtual BlockType getBelowTopBlock() const = 0;
	virtual BlockType getUnderEarth() const = 0;
	virtual BlockType getUnderwaterBlock() const = 0;
	virtual void plantTree(ChunkSection& chunk, int x, int y, int z) = 0;

	int getTreeFrequency() const { return _treeFrequency; }
	int getPlantFrequency() const { return _plantFrequency; }

};

#endif // BIOME_H
