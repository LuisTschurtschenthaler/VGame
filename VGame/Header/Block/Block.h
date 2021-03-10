#ifndef BLOCK_H
#define BLOCK_H

#include <array>
#include "BlockID.h"
#include "MeshTypes.h"
#include "Orientation.h"


struct Block {
	const char* name;
	int blockBreakTexture;
	int textures[6];
	int texturePixelOffset[6];
	MeshType meshType;
	float lightLevel; //float walkDrag, hardness (-1 = unbreakable);
	bool isFloraBlock, hasHitbox, isTransparent, useAmbient, isRotateable;
};


class BlockManager {

public:
	static Block* blocks[BlockID::TOTAL_BLOCKS];

};

#endif // BLOCK_H
