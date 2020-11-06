#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

class Chunk;

/* Noise functions from: https://github.com/Hopson97/MineCraft-One-Week-Challenge */
struct NoiseSettings {
	int octaves;
	int amplitude;
	int smoothness;
	int offset;
	float roughness;
};


class NoiseGenerator {

private:
	NoiseSettings _settings;
	int _seed;

public:
	NoiseGenerator(int seed);
	~NoiseGenerator();

	float getHeight(int x, int z, int chunkX, int chunkZ) const;
	void setNoiseSettings(const NoiseSettings& settings) { _settings = settings; }

private:
	float _getNoise(float n) const;
	float _getNoise(float x, float z) const;

	float _lerp(float a, float b, float z) const;
	float _noise(float x, float z) const;

};

#endif // NOISE_GENERATOR_H
