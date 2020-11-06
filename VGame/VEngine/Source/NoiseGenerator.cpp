#include "NoiseGenerator.h"
#include "WorldConstants.h"
#include "Chunk.h"
#include <cmath>

NoiseGenerator::NoiseGenerator(int seed) 
	: _seed(seed) {

	_settings.octaves = 6;
	_settings.amplitude = 110;
	_settings.smoothness = 900;
	_settings.offset = 0;
	_settings.roughness = 0.5;
}

NoiseGenerator::~NoiseGenerator() {
}


float NoiseGenerator::getHeight(int x, int z, int chunkX, int chunkZ) const {
	float newX = x + chunkX * CHUNK_SIZE,
		  newZ = z + chunkZ * CHUNK_SIZE;

	float value = 0.f;
	for(int i = 0; i < _settings.octaves; i++) {
		float frequency = pow(2.f, i);
		float amplitude = pow(_settings.roughness, i);
		
		value += _noise(static_cast<float>(newX) * frequency / _settings.smoothness, (static_cast<float>(newZ) * frequency / _settings.smoothness)) * amplitude;
	}

	float val = (((value / 2.1f) + 1.2f) * _settings.amplitude) + _settings.offset;
	return (val > 0.f) ? val : 1.f;
}


float NoiseGenerator::_getNoise(float n) const {
	int newN = static_cast<int>(n);
	newN += _seed;
	newN = (newN << 13) ^ newN;
	newN = (newN * (newN * newN * 60493 + 19990303) + 1376312589) & 0x7fffffff;

	return 1.f - (static_cast<float>(newN) / 1073741824.f);
}

float NoiseGenerator::_getNoise(float x, float z) const{
	return _getNoise(x + z * 57.f);
}

float NoiseGenerator::_lerp(float a, float b, float z) const{
	float lerp = (1.f - std::cos(z * PI)) / 2.f;
	return (a * (1.f - lerp) + b * lerp);
}

float NoiseGenerator::_noise(float x, float z) const {
	float floorX = std::floor(x);
	float floorZ = std::floor(z);

	float s = 0.f, t = 0.f, u = 0.f, v = 0.f;
	s = _getNoise(floorX, floorZ);
	t = _getNoise(floorX + 1, floorZ);
	u = _getNoise(floorX, floorZ + 1);
	v = _getNoise(floorX + 1, floorZ + 1);

	float rec1 = _lerp(s, t, x - floorX);
	float rec2 = _lerp(u, v, x - floorX);
	return _lerp(rec1, rec2, z - floorZ);
}
