#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ChunkManager.h"
#include "Shader.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"


ParticleSystem::ParticleSystem() {
	_shader = new Shader("particle_vert.glsl", "particle_frag.glsl");
}

ParticleSystem::~ParticleSystem() {
	delete _shader;

	for(auto& particleEmitter : _particleEmitters)
		delete particleEmitter;
}


void ParticleSystem::update() {
	_shader->setMat4("projection", World::getPlayer().camera->getProjection());
	_shader->setMat4("view", World::getPlayer().camera->getView());
	_shader->setInt("textureAtlas", World::getChunkManager().getTextureAtlasID());

	for(int i = 0; i < _particleEmitters.size(); i++) {
		_particleEmitters[i]->update();

		if(_particleEmitters[i]->isFinished()) {
			delete _particleEmitters[i];

			_particleEmitters.erase(_particleEmitters.begin() + i);
			--i;
		}
	}
}

void ParticleSystem::draw() {
	_shader->bind();

	for(auto& particleEmitter : _particleEmitters)
		particleEmitter->draw();

	_shader->unbind();
}

void ParticleSystem::addParticleEmitter(ParticleEmitter* particleEmitter) {
	_particleEmitters.push_back(particleEmitter);
}
