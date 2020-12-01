#include "ChunkSection.h"
#include "ChunkManager.h"
#include "ChunkMesh.h"
#include "Chunk.h"
#include "WorldConstants.h"
#include "TerrainGenerator.h"
#include "Block.h"


ChunkSection::ChunkSection(ChunkManager* chunkManager, Chunk* chunk, const ChunkCoordXYZ& coord)
	: coord(coord), areMeshesGenerated(false), chunkManager(chunkManager), _chunk(chunk) {

	aabb = AABB({ CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE });
	aabb.update({ coord.x * CHUNK_SIZE, coord.y * CHUNK_SIZE, coord.z * CHUNK_SIZE });
	data.fill(BlockType::AIR);
}

ChunkSection::~ChunkSection() {
}


void ChunkSection::placeBlock(const BlockPositionXYZ& blockCoord, const BlockType& block) {
	data.set(blockCoord.x, blockCoord.y, blockCoord.z, block);
}

BlockType ChunkSection::getBlock(const BlockPositionXYZ& blockCoord) {
	return data.get(blockCoord.x, blockCoord.y, blockCoord.z);
}

void ChunkSection::generateMesh() {
 	for(int x = 0; x < CHUNK_SIZE; x++) {
		for(int z = 0; z < CHUNK_SIZE; z++) {
			for(int y = 0; y < CHUNK_SIZE; y++) {

				BlockType blockType = data.get(x, y, z);
				if(blockType != BlockType::AIR) {
					
					Block* block = BlockUtil::blocks[blockType];
					switch(block->meshType) {
						case MeshType::SOLID:
							_addBlockFaces(x, y, z, MeshType::SOLID, block);
							break;


						case MeshType::FLORA:
							if(block->name == "Oak leave") {
								//for(int i = 0; i < AMOUNT_OF_BLOCKFACES; i++)
								//	_chunk->meshCollection[FLORA]->addBlockFace(this, x, y, z, static_cast<BlockFace>(i), block);

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

			}
		}
	}

	areMeshesGenerated = true;
}

Block* ChunkSection::getBlockRelative(int x, int y, int z) {
	// Right -> X+
	if(x > CHUNK_SIZE - 1
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
			->getChunkSection(coord.y)->_getBlock(CHUNK_SIZE - 1, y, z);
	}

	// Top -> Y+
	else if(x < CHUNK_SIZE
			&& x >= 0
			&& y > CHUNK_SIZE - 1
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
			return _chunk->getChunkSection(coord.y - 1)->_getBlock(x, CHUNK_SIZE - 1, z);
		else return BlockUtil::blocks[STONE];
	}

	// Front -> Z+
	else if(x < CHUNK_SIZE
			&& x >= 0
			&& y < CHUNK_SIZE
			&& y >= 0
			&& z > CHUNK_SIZE - 1) {

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
			->getChunkSection(coord.y)->_getBlock(x, y, CHUNK_SIZE - 1);
	}

	else if(!(x >= 0
			&& x < CHUNK_SIZE
			&& y >= 0
			&& y < CHUNK_SIZE
			&& z >= 0
			&& z < CHUNK_SIZE)) {

		return BlockUtil::blocks[AIR];
	}

	else return _getBlock(x, y, z);
}


void ChunkSection::_addBlockFaces(int x, int y, int z, MeshType meshType, Block* block) {
	Block* relativeBlock = getBlockRelative(x + 1, y, z);
	if(relativeBlock->isTransparent || relativeBlock->meshType == MeshType::FLORA)
		_chunk->meshCollection[meshType]->addBlockFace(this, x, y, z, FACE_RIGHT, block);

	relativeBlock = getBlockRelative(x - 1, y, z);
	if(relativeBlock->isTransparent || relativeBlock ->meshType == MeshType::FLORA)
		_chunk->meshCollection[meshType]->addBlockFace(this, x, y, z, FACE_LEFT, block);

	relativeBlock = getBlockRelative(x, y + 1, z);
	if(relativeBlock->isTransparent || relativeBlock->meshType == MeshType::FLORA)
		_chunk->meshCollection[meshType]->addBlockFace(this, x, y, z, FACE_TOP, block);

	relativeBlock = getBlockRelative(x, y - 1, z);
	if(relativeBlock->isTransparent || relativeBlock->meshType == MeshType::FLORA)
		_chunk->meshCollection[meshType]->addBlockFace(this, x, y, z, FACE_BOTTOM, block);

	relativeBlock = getBlockRelative(x, y, z + 1);
	if(relativeBlock->isTransparent || relativeBlock->meshType == MeshType::FLORA)
		_chunk->meshCollection[meshType]->addBlockFace(this, x, y, z, FACE_FRONT, block);

	relativeBlock = getBlockRelative(x, y, z - 1);
	if(relativeBlock->isTransparent || relativeBlock->meshType == MeshType::FLORA)
		_chunk->meshCollection[meshType]->addBlockFace(this, x, y, z, FACE_BACK, block);
}

bool ChunkSection::_isOutOfChunkRange(BlockPositionXYZ coord) {
	return (_isOutOfChunkRange(coord.x) || _isOutOfChunkRange(coord.y) || _isOutOfChunkRange(coord.z));
}

bool ChunkSection::_isOutOfChunkRange(int nr) {
	return (nr >= CHUNK_SIZE || nr < 0);
}

Block* ChunkSection::_getBlock(int x, int y, int z) {
	return BlockUtil::blocks[data.get(x, y, z)];
}
