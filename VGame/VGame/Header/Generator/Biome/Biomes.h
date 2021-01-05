#ifndef BIOMES_H
#define BIOMES_H

#include "Biome.h"
#include "BlockID.h"
class ChunkSection;


class Desert
	: public Biome {

public:
	Desert(int seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;
	
	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class Grassland
	: public Biome {

public:
	Grassland(int seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class Forest
	: public Biome {

public:
	Forest(int seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class SnowForest
	: public Biome {

public:
	SnowForest(int seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class HighLands
	: public Biome {

public:
	HighLands(int seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};

#endif // BIOMES_H
