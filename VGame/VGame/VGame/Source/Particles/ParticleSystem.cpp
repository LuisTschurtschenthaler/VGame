#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "ChunkManager.h"
#include "Shader.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Game.h"


ParticleSystem::ParticleSystem() {
}

ParticleSystem::~ParticleSystem() {
	for(auto& particleEmitter : _particleEmitters)
		delete particleEmitter;
}


void ParticleSystem::update() {
	Game::basicShader->setMat4("projection", World::getPlayer().camera->getProjection());
	Game::basicShader->setMat4("view", World::getPlayer().camera->getView());
	Game::basicShader->setInt("textureAtlas", 0);

	for(int i = 0; i < _particleEmitters.size(); i++) {
		_particleEmitters[i]->update();

		if(_particleEmitters[i]->isFinished()) {
			delete _particleEmitters[i];

			_particleEmitters.erase(_particleEmitters.begin() + i);
			i--;
		}
	}
}

void ParticleSystem::draw() {
	Game::basicShader->bind();

	for(auto& particleEmitter : _particleEmitters)
		particleEmitter->draw();

	Game::basicShader->unbind();
}

void ParticleSystem::addParticleEmitter(ParticleEmitter* particleEmitter) {
	_particleEmitters.push_back(particleEmitter);
}
