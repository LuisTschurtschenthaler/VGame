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
};


class BirchForest
	: public Biome {

public:
	BirchForest(int seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;
};


class JungleForest
	: public Biome {

public:
	JungleForest(int seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;
};


#endif // BIOMES_H
