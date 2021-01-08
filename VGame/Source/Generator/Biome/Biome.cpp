#include "Biome.h"
#include "Random.h"


Biome::Biome(int treeFrequency, int plantFrequency, int seed)
	: _treeFrequency(treeFrequency), _plantFrequency(plantFrequency) {

	_noise = new NoiseGenerator(seed);

	NoiseSettings settings;
	settings.octaves = 15;
	settings.amplitude = 55;
	settings.smoothness = 160;
	settings.offset = 10;
	settings.roughness = 0.45f;
	_noise->setNoiseSettings(settings);
}

Biome::~Biome() {
}

int Biome::getHeight(int x, int z, int chunkX, int chunkZ) {
	return static_cast<int>(_noise->getNoise(x, z, chunkX, chunkZ));
}
