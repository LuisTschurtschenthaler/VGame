#include "Biomes.h"
#include "Biomes.h"
#include "Random.h"


Desert::Desert(int seed)
	: Biome(200, 100, seed) {
}

std::string Desert::getName() const {
	return "Desert";
}

BlockID Desert::getPlant() const {
	return BlockID::DEAD_BUSH;
}

BlockID Desert::getTopBlock() const {
	return BlockID::SAND;
}

BlockID Desert::getBelowTopBlock() const {
	return BlockID::SAND;
}

BlockID Desert::getUnderEarth() const {
	return BlockID::SANDSTONE;
}

BlockID Desert::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockID::GRAVEL : BlockID::SAND;
}


Grassland::Grassland(int seed)
	: Biome(1000, 15, seed) {
}

std::string Grassland::getName() const {
	return "Grassland";
}


BlockID Grassland::getPlant() const {
	int random = Random::get(0, 10);

	if(random <= 2)
		return BlockID::TALL_GRASS;
	if(random <= 4)
		return BlockID::GRASS2;
	if(random <= 6)
		return BlockID::DANDELION;
	if(random <= 8)
		return BlockID::POPPY;
	else
		return BlockID::BLUE_ORCHID;
}

BlockID Grassland::getTopBlock() const {
	return BlockID::GRASS;
}

BlockID Grassland::getBelowTopBlock() const {
	return BlockID::DIRT;
}

BlockID Grassland::getUnderEarth() const {
	return BlockID::STONE;
}

BlockID Grassland::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockID::GRAVEL : BlockID::SAND;
}


Forest::Forest(int seed)
	: Biome(30, 15, seed) {
}

std::string Forest::getName() const {
	return "Forest";
}

BlockID Forest::getPlant() const {
	return (Random::get(0, 10) > 8) ? BlockID::TALL_GRASS : BlockID::GRASS2;
}

BlockID Forest::getTopBlock() const {
	return BlockID::GRASS;
}

BlockID Forest::getBelowTopBlock() const {
	return BlockID::DIRT;
}

BlockID Forest::getUnderEarth() const {
	return BlockID::STONE;
}

BlockID Forest::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockID::GRAVEL : BlockID::SAND;
}


BirchForest::BirchForest(int seed)
	: Biome(30, 15, seed) {
}

std::string BirchForest::getName() const {
	return "Birch forest";
}

BlockID BirchForest::getPlant() const {
	return (Random::get(0, 10) > 8) ? BlockID::TALL_GRASS : BlockID::GRASS2;
}

BlockID BirchForest::getTopBlock() const {
	return BlockID::GRASS;
}

BlockID BirchForest::getBelowTopBlock() const {
	return BlockID::DIRT;
}

BlockID BirchForest::getUnderEarth() const {
	return BlockID::STONE;
}

BlockID BirchForest::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockID::GRAVEL : BlockID::SAND;
}


JungleForest::JungleForest(int seed)
	: Biome(30, 15, seed) {
}

std::string JungleForest::getName() const {
	return "Jungle forest";
}

BlockID JungleForest::getPlant() const {
	return (Random::get(0, 10) > 8) ? BlockID::TALL_GRASS : BlockID::GRASS2;
}

BlockID JungleForest::getTopBlock() const {
	return BlockID::GRASS;
}

BlockID JungleForest::getBelowTopBlock() const {
	return BlockID::DIRT;
}

BlockID JungleForest::getUnderEarth() const {
	return BlockID::STONE;
}

BlockID JungleForest::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockID::GRAVEL : BlockID::SAND;
}
