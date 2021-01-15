#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Chunk.h"
#include "World.h"
#include "ChunkManager.h"
#include "TerrainGenerator.h"


Chunk::Chunk(ChunkManager* chunkManager, const ChunkXZ& coord)
	: chunkManager(chunkManager), coord(coord), worldCoord(coord * CHUNK_SIZE) {

	solid = new ChunkMesh(this);
	fluid = new ChunkMesh(this);
	
	chunkData.fill(BlockID::AIR);
	chunkDataGenerated = false;
	meshesGenerated = false;
	nearbyChunksDetected = false;
	isDirty = false;
}

Chunk::~Chunk() {
	solid->clear();
	delete solid;

	fluid->clear();
	delete fluid;
}


void Chunk::drawSolid() {
	solid->draw();
}

void Chunk::drawFluid() {
	fluid->draw();
}

void Chunk::generateChunkData() {
	World::terrainGenerator->generateChunkData(coord, chunkData);
	World::terrainGenerator->generateFlora(coord, chunkData);
	chunkDataGenerated = true;
}

void Chunk::generateChunkMesh(ChunkMesh* solid, ChunkMesh* fluid) {
	ChunkMesh* solidMesh = (solid == nullptr) ? this->solid : solid;
	ChunkMesh* fluidMesh = (fluid == nullptr) ? this->fluid : fluid;

	if(!nearbyChunksDetected) {
		chunkManager->getNearbyChunks(coord, nearbyChunks);
		nearbyChunksDetected = true;
	}
	
	for(auto& chunk : nearbyChunks) {
		if(!chunk->chunkDataGenerated)
			chunk->generateChunkData();
	}

	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++)
	for(int y = 0; y < CHUNK_HEIGHT; y++) {
		
		BlockID blockID = chunkData.get(x, y, z);
		if(blockID == BlockID::AIR)
			continue;

		Block* block = BlockUtil::blocks[blockID];
		switch(block->meshType) {
			
			case MeshType::SOLID:
				if(block->isFloraBlock && block->name != "Oak leave" && block->name != "Cactus") {
					if(block->name == "Tall grass") {
						solidMesh->addFloraBlock(this, x, y, z, BlockFace::FACE_BOTTOM, block);
						solidMesh->addFloraBlock(this, x, y + 1, z, BlockFace::FACE_TOP, block);
					}
					else solidMesh->addFloraBlock(this, x, y, z, BlockFace::FACE_FRONT, block);
				}
				else solidMesh->addBlock(this, x, y, z, block);
				break;

			case MeshType::FLUID:
				if(y == WATER_LEVEL)
					fluidMesh->addBlockFace(this, x, y, z, BlockFace::FACE_TOP, block);
				break;
		}
	}
	meshesGenerated = true;
}

void Chunk::recreateChunkMesh() {
	ChunkMesh* solidMesh = new ChunkMesh(this); 
	ChunkMesh* fluidMesh = new ChunkMesh(this);
	generateChunkMesh(solidMesh, fluidMesh);
	
	this->solid->clear();
	this->fluid->clear();
	this->solid = solidMesh;
	this->fluid = fluidMesh;
	isDirty = false;
}

const glm::mat4& Chunk::getModel() {
	glm::mat4 model(1.f);
	model = glm::translate(model, glm::vec3(worldCoord.x, 0, worldCoord.z));
	model = glm::rotate(model, glm::radians(0.f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(0.f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(0.f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(1.f));
	return model;
}

const Block* Chunk::getBlockRelative(const LocationXYZ& loc) const {
	// Right -> X+
	if(loc.x >= CHUNK_SIZE
	   && loc.y < CHUNK_HEIGHT
	   && loc.y >= 0
	   && loc.z < CHUNK_SIZE
	   && loc.z >= 0) {

		return nearbyChunks[CHUNK_RIGHT]->_getBlock(0, loc.y, loc.z);
	}

	// Left -> X-
	else if(loc.x < 0
			&& loc.y < CHUNK_HEIGHT
			&& loc.y >= 0
			&& loc.z < CHUNK_SIZE
			&& loc.z >= 0) {

		return nearbyChunks[CHUNK_LEFT]->_getBlock(CHUNK_SIZE - 1, loc.y, loc.z);
	}

	// Top -> Y+
	else if(loc.x < CHUNK_SIZE
			&& loc.x >= 0
			&& loc.y >= CHUNK_HEIGHT
			&& loc.z < CHUNK_SIZE
			&& loc.z >= 0) {
		
		return BlockUtil::blocks[AIR];
	}

	// Bottom -> Y- 
	else if(loc.x < CHUNK_SIZE
			&& loc.x >= 0
			&& loc.y < 0
			&& loc.z < CHUNK_SIZE
			&& loc.z >= 0) {

		return BlockUtil::blocks[ERROR];
	}

	// Front -> Z+
	else if(loc.x < CHUNK_SIZE
			&& loc.x >= 0 
			&& loc.y < CHUNK_HEIGHT
			&& loc.y >= 0
			&& loc.z >= CHUNK_SIZE) {

		return nearbyChunks[CHUNK_FRONT]->_getBlock(loc.x, loc.y, 0);
	}

	// Back -> Z-
	else if(loc.x < CHUNK_SIZE
			&& loc.x >= 0
			&& loc.y < CHUNK_HEIGHT
			&& loc.y >= 0
			&& loc.z < 0) {

		return nearbyChunks[CHUNK_BACK]->_getBlock(loc.x, loc.y, CHUNK_SIZE - 1);
	}

	else if(!(loc.x >= 0
			&& loc.x < CHUNK_SIZE
			&& loc.y >= 0
			&& loc.y < CHUNK_HEIGHT
			&& loc.z >= 0
			&& loc.z < CHUNK_SIZE)) {

		Chunk* chunk = chunkManager->getChunk({ loc.x << 4, loc.z << 4 });
		return chunk->_getBlock(chunkManager->getBlockLocation(loc));
	}

	// This chunk
	else return _getBlock(loc);
}

const Block* Chunk::getBlockRelative(const int& x, const int& y, const int& z) const {
	return getBlockRelative({ x, y, z });
}

const Block* Chunk::_getBlock(const LocationXYZ& location) const {
	return BlockUtil::blocks[chunkData.get(location)];
}

const Block* Chunk::_getBlock(const int& x, const int& y, const int& z) const {
	return _getBlock({ x, y, z });
}
