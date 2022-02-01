#ifndef BIOMES_H
#define BIOMES_H

#include "Biome.h"
#include "BlockID.h"
class ChunkSection;


class Desert
	: public Biome {

public:
	Desert(const int& seed);

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
	Grassland(const int& seed);

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
	Forest(const int& seed);

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
	BirchForest(const int& seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;
};


class SnowForest
	: public Biome {

public:
	SnowForest(const int& seed);

	std::string getName() const override;
	BlockID getPlant() const override;
	BlockID getTopBlock() const override;
	BlockID getBelowTopBlock() const override;
	BlockID getUnderEarth() const override;
	BlockID getUnderwaterBlock() const override;
};


#endif // BIOMES_H
