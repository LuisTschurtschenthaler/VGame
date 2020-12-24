#ifndef BLOCK_H
#define BLOCK_H

#include "BlockType.h"
#include "Coordinates.h"
#include "AABB.h"


enum BlockFace {
	FACE_RIGHT	= 0, // X+
	FACE_LEFT	= 1, // X-
	FACE_TOP	= 2, // Y+
	FACE_BOTTOM	= 3, // Y-
	FACE_FRONT	= 4, // Z+
	FACE_BACK	= 5, // Z-

	AMOUNT_OF_BLOCKFACES
};


struct Block {
	const char* name;
	int textures[6];
	float texturePixelOffset, lightLevel; //float walkDrag;
	int meshType;
	bool isFloraBlock, hasHitbox;
};


class BlockUtil {

public:
	static Block* blocks[AMOUNT_OF_BLOCKS];

};

#endif // BLOCK_H
