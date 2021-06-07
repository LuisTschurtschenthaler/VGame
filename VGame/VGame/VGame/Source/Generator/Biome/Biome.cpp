#include "Biome.h"
#include "NoiseGenerator.h"
#include "Random.h"


Biome::Biome(const int& treeFrequency, const int& plantFrequency, const int& seed)
	: _treeFrequency(treeFrequency), _plantFrequency(plantFrequency) {

	_noise = new NoiseGenerator(seed);

	NoiseSettings settings;
	settings.octaves = 11;
	settings.amplitude = 75;
	settings.smoothness = 150;
	settings.offset = 15;
	settings.roughness = 0.45f;
	_noise->setNoiseSettings(settings);
}

Biome::~Biome() {
	delete _noise;
}

int Biome::getHeight(const int& x, const int& z, const int& chunkX, const int& chunkZ) {
	return static_cast<int>(_noise->getNoise(x, z, chunkX, chunkZ));
}
