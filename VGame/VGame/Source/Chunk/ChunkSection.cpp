#include "ChunkSection.h"
#include "ChunkManager.h"
#include "ChunkMesh.h"
#include "Chunk.h"
#include "WorldConstants.h"
#include "TerrainGenerator.h"
#include "Block.h"


ChunkSection::ChunkSection(ChunkManager* chunkManager, Chunk* chunk, const ChunkCoordXYZ& coord)
	: coord(coord), areMeshesGenerated(false), chunkManager(chunkManager), _chunk(chunk) {

	data.fill(BlockType::AIR);
}

ChunkSection::~ChunkSection() {
}


void ChunkSection::placeBlock(const BlockPositionXYZ& blockCoord, const BlockType& block) {
	data.set(blockCoord.x, blockCoord.y, blockCoord.z, block);
}

void ChunkSection::removeBlock(const BlockPositionXYZ& blockCoord) {
	data.set(blockCoord.x, blockCoord.y, blockCoord.z, BlockType::AIR);
}

BlockType ChunkSection::getBlock(const BlockPositionXYZ& blockCoord) {
	return data.get(blockCoord.x, blockCoord.y, blockCoord.z);
}

void ChunkSection::generateMesh() {
	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++)
	for(int y = 0; y < CHUNK_SIZE; y++) {

		BlockType blockType = data.get(x, y, z);
		if(blockType == BlockType::AIR)
			continue;

		Block* block = BlockUtil::blocks[blockType];
		switch(block->meshType) {
			case MeshType::SOLID:
				_addBlockFaces(x, y, z, MeshType::SOLID, block);
				break;


			case MeshType::FLORA:
				if(block->name == "Oak leave") {
					_addBlockFaces(x, y, z, MeshType::FLORA, block);
					break;
				}

				else if(block->name == "Tall grass") {
					_chunk->meshCollection[FLORA]->addFloraBlock(this, x, y, z, BlockFace::FACE_BOTTOM, block);
					_chunk->meshCollection[FLORA]->addFloraBlock(this, x, y + 1, z, BlockFace::FACE_TOP, block);
				}

				else _chunk->meshCollection[FLORA]->addFloraBlock(this, x, y, z, BlockFace::FACE_FRONT, block);
				break;


			case MeshType::FLUID:
				if((y + coord.y * CHUNK_SIZE) == WATER_LEVEL)
					_chunk->meshCollection[FLUID]->addBlockFace(this, x, y, z, FACE_TOP, block);
				break;
		}
	}

	areMeshesGenerated = true;
}

const Block* ChunkSection::getBlockRelative(int x, int y, int z) const {
	// Right -> X+
	if(x > CHUNK_SIZE_R
	   && y < CHUNK_SIZE
	   && y >= 0
	   && z < CHUNK_SIZE
	   && z >= 0) {

		return _chunk->nearbyChunks[CHUNK_RIGHT]
			->getChunkSection(coord.y)->_getBlock(0, y, z);
	}

	// Left -> X-
	else if(x < 0
			&& y < CHUNK_SIZE
			&& y >= 0
			&& z < CHUNK_SIZE
			&& z >= 0) {

		return _chunk->nearbyChunks[CHUNK_LEFT]
			->getChunkSection(coord.y)->_getBlock(CHUNK_SIZE_R, y, z);
	}

	// Top -> Y+
	else if(x < CHUNK_SIZE
			&& x >= 0
			&& y > CHUNK_SIZE_R
			&& z < CHUNK_SIZE
			&& z >= 0) {

		if(coord.y + 1 < CHUNK_SECTIONS - 1)
			return _chunk->getChunkSection(coord.y + 1)->_getBlock(x, 0, z);
		else return BlockUtil::blocks[AIR];
	}

	// Bottom -> Y- 
	else if(x < CHUNK_SIZE
			&& x >= 0
			&& y < 0
			&& z < CHUNK_SIZE
			&& z >= 0) {

		if(coord.y - 1 >= 0)
			return _chunk->getChunkSection(coord.y - 1)->_getBlock(x, CHUNK_SIZE_R, z);
		else return BlockUtil::blocks[STONE];
	}

	// Front -> Z+
	else if(x < CHUNK_SIZE
			&& x >= 0
			&& y < CHUNK_SIZE
			&& y >= 0
			&& z > CHUNK_SIZE_R) {

		return _chunk->nearbyChunks[CHUNK_FRONT]
			->getChunkSection(coord.y)->_getBlock(x, y, 0);
	}

	// Back -> Z-
	else if(x < CHUNK_SIZE
			&& x >= 0
			&& y < CHUNK_SIZE
			&& y >= 0
			&& z < 0) {

		return _chunk->nearbyChunks[CHUNK_BACK]
			->getChunkSection(coord.y)->_getBlock(x, y, CHUNK_SIZE_R);
	}

	else if(!(x >= 0
			&& x < CHUNK_SIZE
			&& y >= 0
			&& y < CHUNK_SIZE
			&& z >= 0
			&& z < CHUNK_SIZE)) {

		return BlockUtil::blocks[chunkManager->getBlock({ x, y, z })];
	}

	// This chunk
	else return _getBlock(x, y, z);
}


void ChunkSection::_addBlockFaces(int x, int y, int z, MeshType meshType, Block* block) {
	std::vector<BlockPositionXYZ> adjacents = {
		{  1,  0,  0 },
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{  0, -1,  0 },
		{  0,  0,  1 },
		{  0,  0, -1 }
	};

	for(uint8_t i = 0; i < adjacents.size(); i++) {
		BlockPositionXYZ blockPos = { x + adjacents[i].x, y + adjacents[i].y, z + adjacents[i].z };
		const Block* relativeBlock = getBlockRelative(blockPos.x, blockPos.y, blockPos.z);
		
		if(relativeBlock->isTransparent || relativeBlock->meshType == MeshType::FLORA)
			_chunk->meshCollection[meshType]->addBlockFace(this, x, y, z, static_cast<BlockFace>(i), block);
	}
}

bool ChunkSection::_isOutOfChunkRange(BlockPositionXYZ coord) {
	return (_isOutOfChunkRange(coord.x) || _isOutOfChunkRange(coord.y) || _isOutOfChunkRange(coord.z));
}

bool ChunkSection::_isOutOfChunkRange(int nr) {
	return (nr >= CHUNK_SIZE || nr < 0);
}

const Block* ChunkSection::_getBlock(int x, int y, int z) const {
	return BlockUtil::blocks[data.get(x, y, z)];
}
