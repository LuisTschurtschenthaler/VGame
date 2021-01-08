#include <GLEW/GL/glew.h>
#include "Chunk.h"
#include "World.h"
#include "ChunkManager.h"
#include "TerrainGenerator.h"


Chunk::Chunk(ChunkManager* chunkManager, const ChunkXZ& coord)
	: chunkManager(chunkManager), coord(coord) {

	solid = new ChunkMesh(this);
	fluid = new ChunkMesh(this);
	
	chunkData.fill(BlockID::AIR);
	chunkDataGenerated = false;
	meshesGenerated = false;
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
	chunkDataGenerated = true;
}

void Chunk::generateChunkMesh() {
	chunkManager->getNearbyChunks(coord, _nearbyChunks);
	
	for(auto& chunk : _nearbyChunks) {
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
				if(block->isFloraBlock) {
					if(block->name == "Tall grass") {
						solid->addFloraBlock(this, x, y, z, BlockFace::FACE_BOTTOM, block);
						solid->addFloraBlock(this, x, y + 1, z, BlockFace::FACE_TOP, block);
					}

					else if(block->name != "Cactus")
						solid->addFloraBlock(this, x, y, z, BlockFace::FACE_FRONT, block);
				}
				else _addBlockFaces({ x, y, z }, MeshType::SOLID, block);
				break;

			case MeshType::FLUID:
				if(y == WATER_LEVEL)
					fluid->addBlockFace(this, x, y, z, BlockFace::FACE_TOP, block);
				break;
		}
	}
	meshesGenerated = true;
}


const Block* Chunk::getBlockRelative(const LocationXYZ& loc) const {
	// Right -> X+
	if(loc.x >= CHUNK_SIZE
	   && loc.y < CHUNK_HEIGHT
	   && loc.y >= 0
	   && loc.z < CHUNK_SIZE
	   && loc.z >= 0) {

		return _nearbyChunks[CHUNK_RIGHT]->_getBlock(0, loc.y, loc.z);
	}

	// Left -> X-
	else if(loc.x < 0
			&& loc.y < CHUNK_HEIGHT
			&& loc.y >= 0
			&& loc.z < CHUNK_SIZE
			&& loc.z >= 0) {

		return _nearbyChunks[CHUNK_LEFT]->_getBlock(CHUNK_SIZE - 1, loc.y, loc.z);
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

		return BlockUtil::blocks[STONE];
	}

	// Front -> Z+
	else if(loc.x < CHUNK_SIZE
			&& loc.x >= 0 
			&& loc.y < CHUNK_HEIGHT
			&& loc.y >= 0
			&& loc.z >= CHUNK_SIZE) {

		return _nearbyChunks[CHUNK_FRONT]->_getBlock(loc.x, loc.y, 0);
	}

	// Back -> Z-
	else if(loc.x < CHUNK_SIZE
			&& loc.x >= 0
			&& loc.y < CHUNK_HEIGHT
			&& loc.y >= 0
			&& loc.z < 0) {

		return _nearbyChunks[CHUNK_BACK]->_getBlock(loc.x, loc.y, CHUNK_SIZE - 1);
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

void Chunk::_addBlockFaces(LocationXYZ loc, MeshType meshType, Block* block) {
	const std::vector<LocationXYZ> adjacents = {
		{  1,  0,  0 },
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{  0, -1,  0 },
		{  0,  0,  1 },
		{  0,  0, -1 }
	};

	for(int i = 0; i < TOTAL_BLOCK_FACES; i++) {
		const Block* relativeBlock = getBlockRelative(loc + adjacents[i]);

		if(!relativeBlock->hasHitbox) {
			if(meshType == MeshType::SOLID)
				solid->addBlockFace(this, loc.x, loc.y, loc.z, static_cast<BlockFace>(i), block);
			else fluid->addBlockFace(this, loc.x, loc.y, loc.z, static_cast<BlockFace>(i), block);
		}
	}
}
