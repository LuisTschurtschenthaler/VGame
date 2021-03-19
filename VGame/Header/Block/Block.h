#ifndef BLOCK_H
#define BLOCK_H

#include <array>
#include "BlockID.h"
#include "MeshTypes.h"
#include "Orientation.h"


struct Block {
	const char* name;
	int blockBreakTexture;
	uint8_t textures[6];
	MeshType meshType;
	float lightLevel; //float walkDrag, hardness (-1 = unbreakable);
	bool isFloraBlock, hasHitbox, isTransparent, useAmbient;
};


class BlockManager {

private:
	static Block _blocks[BlockID::TOTAL_BLOCKS];


public:
	static const Block& getBlock(const uint8_t& blockID);

};

#endif // BLOCK_H
