#ifndef BIOMES_H
#define BIOMES_H

#include "Biome.h"
#include "BlockType.h"
class ChunkSection;


class Desert
	: public Biome {

public:
	Desert(int seed);

	std::string getName() const override;
	BlockType getPlant() const override;
	BlockType getTopBlock() const override;
	BlockType getBelowTopBlock() const override;
	BlockType getUnderEarth() const override;
	BlockType getUnderwaterBlock() const override;
	
	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class Grassland
	: public Biome {

public:
	Grassland(int seed);

	std::string getName() const override;
	BlockType getPlant() const override;
	BlockType getTopBlock() const override;
	BlockType getBelowTopBlock() const override;
	BlockType getUnderEarth() const override;
	BlockType getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class Forest
	: public Biome {

public:
	Forest(int seed);

	std::string getName() const override;
	BlockType getPlant() const override;
	BlockType getTopBlock() const override;
	BlockType getBelowTopBlock() const override;
	BlockType getUnderEarth() const override;
	BlockType getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class SnowForest
	: public Biome {

public:
	SnowForest(int seed);

	std::string getName() const override;
	BlockType getPlant() const override;
	BlockType getTopBlock() const override;
	BlockType getBelowTopBlock() const override;
	BlockType getUnderEarth() const override;
	BlockType getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};


class HighLands
	: public Biome {

public:
	HighLands(int seed);

	std::string getName() const override;
	BlockType getPlant() const override;
	BlockType getTopBlock() const override;
	BlockType getBelowTopBlock() const override;
	BlockType getUnderEarth() const override;
	BlockType getUnderwaterBlock() const override;

	void plantTree(ChunkSection& chunk, int x, int y, int z) override;
};

#endif // BIOMES_H
