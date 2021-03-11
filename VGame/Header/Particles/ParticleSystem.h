#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>

class ParticleEmitter;


class ParticleSystem {

private:
	std::vector<ParticleEmitter*> _particleEmitters;


public:
	ParticleSystem();
	~ParticleSystem();

	void update();
	void draw();

	void addParticleEmitter(ParticleEmitter* particleEmitter);

};

#endif // PARTICLE_SYSTEM_H
