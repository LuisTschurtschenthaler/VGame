#include "Biome.h"
#include "Random.h"

Biome::Biome(int treeFrequency, int plantFrequency, int seed)
	: _treeFrequency(treeFrequency), _plantFrequency(plantFrequency) {

	_noise = new NoiseGenerator(seed);

	NoiseSettings settings;
	settings.octaves = 7;
	settings.amplitude = 70;
	settings.smoothness = 235;
	settings.offset = -5;
	settings.roughness = 0.53;
	_noise->setNoiseSettings(settings);
}

Biome::~Biome() {
}

int Biome::getHeight(int x, int z, int chunkX, int chunkZ) {
	return static_cast<int>(_noise->getHeight(x, z, chunkX, chunkZ));
}
