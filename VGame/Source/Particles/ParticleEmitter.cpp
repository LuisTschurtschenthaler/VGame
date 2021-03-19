#include <cstdlib>
#include <array>
#include <algorithm>
#include <GLEW/GL/glew.h>
#include "ParticleEmitter.h"
#include "CoreEngine.h"
#include "TextureAtlas.h"
#include "Timer.h"
#include "Constants.h"
#include "World.h"
#include "Player.h"
#include "Camera.h"
#include "Random.h"
#include "AABB.h"


ParticleEmitter::ParticleEmitter(const BlockID& blockID, const LocationXYZ& blockLocation, const ParticleSpreading& spreading, const int& amountOfParticles) {
	_particles.resize(amountOfParticles);
	_vertices.resize(amountOfParticles * 4);
	_indices.resize(amountOfParticles * 6);

	auto getRand = []() {
		float r1 = ((rand() % 100) / 100.f - 0.5f),
			  r2 = ((rand() % 100) / 100.f - 0.5f),
			  r3 = ((rand() % 100) / 100.f - 0.5f);
		return glm::vec3(r1, r2, r3);
	};

	glm::vec3 position = { blockLocation.x + 0.5f, blockLocation.y + 0.5f, blockLocation.z + 0.5f };
	
	const Block& block = BlockManager::getBlock(blockID);
	glm::vec2 texCoordsTopLeft = TextureAtlas::getTextureCoords(glm::vec2(0.f, 1.f), block.blockBreakTexture);

	for(int i = 0; i < _particles.size(); i++) {
		float texOffset = (16.f / 255.f) / Random::getIntInRange(24, 32);
		texCoordsTopLeft.x += texOffset;
		texCoordsTopLeft.y -= texOffset;

		_particles[i].position = position + getRand();
		_particles[i].velocity = (getRand() / ((int) spreading / 100.f));
		_particles[i].lifeTime = Random::getFloatInRange(1.f, 1.25f);

		_vertices[(i * 4) + 0].texCoords = glm::vec2(texCoordsTopLeft.x, texCoordsTopLeft.y - texOffset);
		_vertices[(i * 4) + 1].texCoords = glm::vec2(texCoordsTopLeft.x, texCoordsTopLeft.y);
		_vertices[(i * 4) + 2].texCoords = glm::vec2(texCoordsTopLeft.x + texOffset, texCoordsTopLeft.y);
		_vertices[(i * 4) + 3].texCoords = glm::vec2(texCoordsTopLeft.x + texOffset, texCoordsTopLeft.y - texOffset);

		_indices[(i * 6) + 0] = (i * 4) + 0;
		_indices[(i * 6) + 1] = (i * 4) + 1;
		_indices[(i * 6) + 2] = (i * 4) + 2;

		_indices[(i * 6) + 3] = (i * 4) + 0;
		_indices[(i * 6) + 4] = (i * 4) + 2;
		_indices[(i * 6) + 5] = (i * 4) + 3;
	}

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BasicVertex) * _vertices.size(), &_vertices[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
}

ParticleEmitter::~ParticleEmitter() {
}


void ParticleEmitter::update() {
	std::vector<Particle>::iterator it = _particles.begin();

	while(it != _particles.end()) {
		it->lifeTime -= CoreEngine::gameTimer->getDeltaTime();

		if(it->lifeTime <= 0.f)
			it = _particles.erase(it);
		else it++;
	}
}

void ParticleEmitter::draw() {
	glm::mat3 billboard = World::getPlayer().camera->getBillboard();
	float size = 0.1f;

	for(int i = 0; i < _particles.size(); i++) {
		Particle* particle = &_particles[i];
		particle->velocity.y -= (0.5f * CoreEngine::gameTimer->getDeltaTime());

		AABB::collision(*particle, size);
		particle->position += particle->velocity;

		_vertices[(i * 4) + 0].position = billboard * glm::vec3(-size, -size, 0) + particle->position;
		_vertices[(i * 4) + 1].position = billboard * glm::vec3(-size,  size, 0) + particle->position;
		_vertices[(i * 4) + 2].position = billboard * glm::vec3( size,  size, 0) + particle->position;
		_vertices[(i * 4) + 3].position = billboard * glm::vec3( size, -size, 0) + particle->position;

		_indices[(i * 6) + 0] = (i * 4) + 0;
		_indices[(i * 6) + 1] = (i * 4) + 1;
		_indices[(i * 6) + 2] = (i * 4) + 2;

		_indices[(i * 6) + 3] = (i * 4) + 0;
		_indices[(i * 6) + 4] = (i * 4) + 2;
		_indices[(i * 6) + 5] = (i * 4) + 3;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BasicVertex) * _vertices.size(), &_vertices[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (GLvoid*) offsetof(BasicVertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (GLvoid*) offsetof(BasicVertex, texCoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void*) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool ParticleEmitter::isFinished() {
	return (_particles.size() <= 0);
}
