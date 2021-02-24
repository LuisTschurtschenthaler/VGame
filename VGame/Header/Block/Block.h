#ifndef BLOCK_H
#define BLOCK_H

#include <array>
#include "BlockID.h"
#include "MeshTypes.h"
#include "Orientation.h"


enum BlockRotation {
	ROTATION_TB = 0,
	ROTATION_RL,
	ROTATION_FB
};

struct Block {
	const char* name;
	int blockBreakTexture;
	int textures[3][6];
	int texturePixelOffset[6];
	MeshType meshType;
	float lightLevel; //float walkDrag, hardness (-1 = unbreakable);
	bool isFloraBlock, hasHitbox, isTransparent, useAmbient, isRotateable;
};

struct ChunkBlock {
	BlockID blockID;
	BlockRotation rotation;

	ChunkBlock() = default;
	//ChunkBlock(const ChunkBlock&) = delete;

	ChunkBlock(BlockID blockID)
		: blockID(blockID), rotation(BlockRotation::ROTATION_TB) { }

	ChunkBlock(BlockID blockID, BlockRotation rotation)
		: blockID(blockID), rotation(rotation) { }
};


class BlockManager {

public:
	static Block* blocks[BlockID::TOTAL_BLOCKS];

};

#endif // BLOCK_H
