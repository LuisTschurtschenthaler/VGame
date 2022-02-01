#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <vector>
#include <GLM/glm.hpp>
#include "Block.h"
#include "Coordinates.h"
#include "Vertex.h"


struct Particle {
	glm::vec3 position, velocity;
	float lifeTime;

	Particle() 
		: position(glm::vec3(0.f)), velocity(glm::vec3(0.f)), lifeTime(0.f) {
	}
};

enum ParticleSpreading {
	SMALL	= 950,
	NORMAL	= 650,
	BIG		= 400
};


class ParticleEmitter {

private:
	std::vector<Particle> _particles;

	std::vector<BasicVertex> _vertices;
	std::vector<unsigned int> _indices;
	unsigned int _VBO, _IBO;


public:
	ParticleEmitter(const BlockID& blockID, const LocationXYZ& blockLocation, const ParticleSpreading& spreading, const int& amountOfParticles);
	~ParticleEmitter();

	void update();
	void draw();

	bool isFinished();

};

#endif // PARTICLE_EMITTER_H
