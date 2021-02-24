#include <chrono>
#include "Chunk.h"
#include "World.h"
#include "ChunkManager.h"
#include "WorldGenerator.h"
#include "AABB.h"


Chunk::Chunk(ChunkManager* chunkManager, const ChunkXZ& coord)
	: chunkManager(chunkManager), coord(coord), worldCoord(coord * CHUNK_SIZE) {

	_solid = new ChunkMesh(this);
	_fluid = new ChunkMesh(this);
	_transparent = new ChunkMesh(this);
	_aabb = new AABB();
	_aabb->update(worldCoord);

	chunkData.fill({ BlockID::AIR });
	chunkDataGenerated = false;
	meshesGenerated = false;
	nearbyChunksDetected = false;
	isDirty = false;

	minimumPoint = CHUNK_HEIGHT;
	highestPoint = WATER_LEVEL + 1;
}

Chunk::~Chunk() {
	delete _aabb;
	delete _fluid;
	delete _solid;
}


void Chunk::drawSolid() {
	_solid->draw();
}

void Chunk::drawFluid() {
	_fluid->draw();
}

void Chunk::drawTransparent() {
	_transparent->draw();
}

void Chunk::generateChunkMesh(ChunkMesh* solid, ChunkMesh* fluid, ChunkMesh* transparent) {
	ChunkMesh* solidMesh = (solid == nullptr) ? this->_solid : solid;
	ChunkMesh* fluidMesh = (fluid == nullptr) ? this->_fluid : fluid;
	ChunkMesh* transparentMesh = (transparent == nullptr) ? this->_transparent : transparent;

	if(!nearbyChunksDetected) {
		chunkManager->getNearbyChunks(coord, nearbyChunks);
		nearbyChunksDetected = true;
	}

	for(auto& chunk : nearbyChunks) {
		if(!chunk->chunkDataGenerated)
			World::worldGenerator->generateChunk(*chunk);
	}
	
	//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++)
	for(int y = minimumPoint; y < highestPoint; y++) {
		const ChunkBlock& chunkBlock = chunkData.get(x, y, z);
		if(chunkBlock.blockID == BlockID::AIR)
			continue;

		Block* block = BlockManager::blocks[chunkBlock.blockID];
		switch(block->meshType) {
			case MeshType::SOLID:
				if(block->isTransparent)
					transparentMesh->addBlock(this, x, y, z, block, chunkBlock.rotation);

				else if(block->isFloraBlock
						&& block->name != "Oak leave"
						&& block->name != "Birch leave"
						&& block->name != "Jungle leave"
						&& block->name != "Cactus") {

					if(block->name == "Tall grass") {
						solidMesh->addFloraBlock(this, x, y, z, BlockFace::FACE_BOTTOM, block, chunkBlock.rotation);
						solidMesh->addFloraBlock(this, x, y + 1, z, BlockFace::FACE_TOP, block, chunkBlock.rotation);
					}
					else solidMesh->addFloraBlock(this, x, y, z, BlockFace::FACE_FRONT, block, chunkBlock.rotation);
				}
				else solidMesh->addBlock(this, x, y, z, block, chunkBlock.rotation);
				break;

			case MeshType::FLUID:
				fluidMesh->addFluidBlock(this, x, y, z, block, chunkBlock.rotation);
				break;
		}
	}
	meshesGenerated = true;
	//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//std::cout << "Time needed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
}

void Chunk::recreateChunkMesh() {
	ChunkMesh* solid = new ChunkMesh(this); 
	ChunkMesh* fluid = new ChunkMesh(this);
	ChunkMesh* transparent = new ChunkMesh(this);
	generateChunkMesh(solid, fluid, transparent);
	
	_solid->clear();
	_fluid->clear();
	_transparent->clear();

	_solid = solid;
	_fluid = fluid;
	_transparent = transparent;
	isDirty = false;
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
		
		return BlockManager::blocks[ERROR];
	}

	// Bottom -> Y- 
	else if(loc.x < CHUNK_SIZE
			&& loc.x >= 0
			&& loc.y < 0
			&& loc.z < CHUNK_SIZE
			&& loc.z >= 0) {

		return BlockManager::blocks[ERROR];
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

	else if(ChunkManager::isLocationOutOfChunkRange(loc)) {
		Chunk* chunk = chunkManager->getChunk({ loc.x / CHUNK_SIZE, loc.z / CHUNK_SIZE });
		return chunk->_getBlock(chunkManager->getBlockLocation(loc));
	}

	// This chunk
	else return _getBlock(loc);
}

const Block* Chunk::getBlockRelative(const int& x, const int& y, const int& z) const {
	return getBlockRelative({ x, y, z });
}

const Block* Chunk::_getBlock(const LocationXYZ& location) const {
	if(ChunkManager::isLocationOutOfChunkRange(location))
		return BlockManager::blocks[AIR];

 	return BlockManager::blocks[chunkData.get(location).blockID];
}

const Block* Chunk::_getBlock(const int& x, const int& y, const int& z) const {
	return _getBlock({ x, y, z });
}
