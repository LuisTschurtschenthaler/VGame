#include <cmath>
#include "NoiseGenerator.h"
#include "Constants.h"
#include "Chunk.h"


NoiseGenerator::NoiseGenerator(const int& seed)
	: _seed(seed) {

	_settings.octaves = 5;
	_settings.amplitude = 160;
	_settings.smoothness = 1050;
	_settings.offset = 0;
	_settings.roughness = 0.45f;
}

NoiseGenerator::~NoiseGenerator() {
}


float NoiseGenerator::getNoise(int x, int z, int chunkX, int chunkZ) const {
	int	newX = x + chunkX * CHUNK_SIZE,
		newZ = z + chunkZ * CHUNK_SIZE;

	float value = 0.f;
	for(int i = 0; i < _settings.octaves; i++) {
		float frequency = pow(2.f, i);
		float amplitude = pow(_settings.roughness, i);
		
		value += _noise(newX * frequency / _settings.smoothness, (newZ * frequency / _settings.smoothness)) * amplitude;
	}

	float val = (((value / 2.f) + 1.f) * _settings.amplitude) + _settings.offset;
	return (val > 0.f) ? val : 1.f;
}

float NoiseGenerator::getNoise(float x, float y) const {
	int newX = static_cast<int>(x * 1619 + y * 31337);
	newX += _seed;
	newX = (newX << 10) ^ newX;
	newX = (newX * (newX * newX * 60493 + 19990303) + 1376312589) & 0x7FFFFFFF;

	return (1.f - (static_cast<float>(newX) / 1073741824.f));
}

float NoiseGenerator::_linearInterpolation(float x, float y, float z) const {
	float lerp = (1.f - std::cos(z * PI)) / 2.f;
	return (x * (1.f - lerp) + y * lerp);
}

float NoiseGenerator::_noise(float x, float y) const {
	float floorX = std::floor(x);
	float floorY = std::floor(y);

	float n1 = getNoise(floorX,		floorY);
	float n2 = getNoise(floorX + 1, floorY);
	float n3 = getNoise(floorX,		floorY + 1);
	float n4 = getNoise(floorX + 1, floorY + 1);

	float finalNr1 = _linearInterpolation(n1, n2, x - floorX);
	float finalNr2 = _linearInterpolation(n3, n4, x - floorX);
	return _linearInterpolation(finalNr1, finalNr2, y - floorY);
}
