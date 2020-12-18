#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

class Chunk;

/* Similar to: https://github.com/Hopson97/MineCraft-One-Week-Challenge */
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

	float getNoise(int x, int z, int chunkX, int chunkZ) const;

	float getNoise(float x) const;
	float getNoise(float x, float y) const;

	void setNoiseSettings(const NoiseSettings& settings) { _settings = settings; }

private:
	float _linearInterpolation(float x, float y, float z) const;
	float _noise(float x, float y) const;

};

#endif // NOISE_GENERATOR_H
