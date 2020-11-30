#include "Biomes.h"
#include "ChunkSection.h"
#include "Random.h"


Desert::Desert(int seed)
	: Biome(200, 100, seed) {
}

std::string Desert::getName() const {
	return "Desert";
}

BlockType Desert::getPlant() const {
	return BlockType::DEAD_BUSH;
}

BlockType Desert::getTopBlock() const {
	return BlockType::SAND;
}

BlockType Desert::getBelowTopBlock() const {
	return BlockType::SAND;
}

BlockType Desert::getUnderEarth() const {
	return BlockType::SANDSTONE;
}

BlockType Desert::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockType::GRAVEL : BlockType::SAND;
}

void Desert::plantTree(ChunkSection& chunk, int x, int y, int z) {

}


Grassland::Grassland(int seed)
	: Biome(1000, 15, seed) {
}

std::string Grassland::getName() const {
	return "Grassland";
}


BlockType Grassland::getPlant() const {
	return (Random::get(0, 10) > 8) ? BlockType::TALL_GRASS : BlockType::GRASS2;
}

BlockType Grassland::getTopBlock() const {
	return BlockType::GRASS;
}

BlockType Grassland::getBelowTopBlock() const {
	return BlockType::DIRT;
}

BlockType Grassland::getUnderEarth() const {
	return BlockType::STONE;
}

BlockType Grassland::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockType::GRAVEL : BlockType::SAND;
}

void Grassland::plantTree(ChunkSection& chunk, int x, int y, int z) {

}


Forest::Forest(int seed)
	: Biome(150, 15, seed) {
}

std::string Forest::getName() const {
	return "Forest";
}

BlockType Forest::getPlant() const {
	return (Random::get(0, 10) > 8) ? BlockType::TALL_GRASS : BlockType::GRASS2;
}

BlockType Forest::getTopBlock() const {
	return BlockType::GRASS;
}

BlockType Forest::getBelowTopBlock() const {
	return BlockType::DIRT;
}

BlockType Forest::getUnderEarth() const {
	return BlockType::STONE;
}

BlockType Forest::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockType::GRAVEL : BlockType::SAND;
}

void Forest::plantTree(ChunkSection& chunk, int x, int y, int z) {

}


SnowForest::SnowForest(int seed)
	: Biome(150, 15, seed) {
}

std::string SnowForest::getName() const {
	return "SnowForest";
}

BlockType SnowForest::getPlant() const {
	return (Random::get(0, 10) > 8) ? BlockType::TALL_GRASS : BlockType::GRASS2;
}

BlockType SnowForest::getTopBlock() const {
	return BlockType::SNOW; // SNOWGRASS
}

BlockType SnowForest::getBelowTopBlock() const {
	return BlockType::DIRT;
}

BlockType SnowForest::getUnderEarth() const {
	return BlockType::STONE;
}

BlockType SnowForest::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockType::DIRT : BlockType::SAND;
}

void SnowForest::plantTree(ChunkSection& chunk, int x, int y, int z) {

}


HighLands::HighLands(int seed)
	: Biome(150, 100, seed) {
}

std::string HighLands::getName() const {
	return "HighLands";
}

BlockType HighLands::getPlant() const {
	return BlockType::GRASS2;
}

BlockType HighLands::getTopBlock() const {
	return (Random::get(0, 10) > 9) ? BlockType::GRAVEL : BlockType::DIRT;
}

BlockType HighLands::getBelowTopBlock() const {
	return BlockType::DIRT;
}

BlockType HighLands::getUnderEarth() const {
	return BlockType::STONE;
}

BlockType HighLands::getUnderwaterBlock() const {
	return (Random::get(0, 10) > 8) ? BlockType::DIRT : BlockType::SAND;
}

void HighLands::plantTree(ChunkSection& chunk, int x, int y, int z) {

}
