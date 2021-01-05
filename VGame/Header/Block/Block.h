#ifndef BLOCK_H
#define BLOCK_H

#include "BlockID.h"
#include "MeshTypes.h"
#include "Orientation.h"


struct Block {
	const char* name;
	int textures[6], texturePixelOffset[6];
	MeshType meshType;
	float lightLevel; //float walkDrag;
	bool isFloraBlock, hasHitbox;
};

class BlockUtil {

public:
	static Block* blocks[TOTAL_BLOCKS];

};

#endif // BLOCK_H
