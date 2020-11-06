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


void ChunkSection::placeBlock(BlockPositionXYZ blockCoord, const BlockType& block) {
	if(_isOutOfChunkRange(blockCoord)) {
		chunkManager->placeBlock(blockCoord, block);
		return;
	}

	data.set(blockCoord.x, blockCoord.y, blockCoord.z, block);
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
							if(_getBlockRelative(x + 1, y, z)->isTransparent)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, FACE_RIGHT, block);

							if(_getBlockRelative(x - 1, y, z)->isTransparent)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, FACE_LEFT, block);

							if(_getBlockRelative(x, y + 1, z)->isTransparent)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, FACE_TOP, block);

							if(_getBlockRelative(x, y - 1, z)->isTransparent)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, FACE_BOTTOM, block);

							if(_getBlockRelative(x, y, z + 1)->isTransparent)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, FACE_FRONT, block);

							if(_getBlockRelative(x, y, z - 1)->isTransparent)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, FACE_BACK, block);
							break;


						case MeshType::FLUID:
							if((y + coord.y * CHUNK_SIZE) == WATER_LEVEL)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, FACE_TOP, block);
							break;


						case MeshType::FLORA:
							for(int i = 0; i < AMOUNT_OF_BLOCKFACES; i++)
								_chunk->meshCollection[SOLID]->addBlockFace(this, x, y, z, static_cast<BlockFace>(i), block);
							break;
					}
				}

			}
		}
	}

	areMeshesGenerated = true;
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

Block* ChunkSection::_getBlockRelative(int x, int y, int z) {
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
