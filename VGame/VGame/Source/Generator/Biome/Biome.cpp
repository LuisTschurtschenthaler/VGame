#include "Biome.h"
#include "Random.h"


Biome::Biome(int treeFrequency, int plantFrequency, int seed)
	: _treeFrequency(treeFrequency), _plantFrequency(plantFrequency) {

	_noise = new NoiseGenerator(seed);

	NoiseSettings settings;
	settings.octaves = 6;
	settings.amplitude = 50;
	settings.smoothness = 145;
	settings.offset = 0;
	settings.roughness = 0.45f;
	_noise->setNoiseSettings(settings);
}

Biome::~Biome() {
}

int Biome::getHeight(int x, int z, int chunkX, int chunkZ) {
	return static_cast<int>(_noise->getHeight(x, z, chunkX, chunkZ));
}
