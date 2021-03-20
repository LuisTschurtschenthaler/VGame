#ifndef BLOCK_H
#define BLOCK_H

#include "BlockID.h"
#include "MeshTypes.h"


struct Block {
	const char* name;
	int blockBreakTexture;
	unsigned char textures[6];
	MeshType meshType;
	float lightLevel; //float walkDrag, hardness (-1 = unbreakable);
	bool isFloraBlock, hasHitbox, isTransparent, useAmbient;
};


class BlockManager {

private:
	static const Block _blocks[BlockID::TOTAL_BLOCKS];


public:
	static const Block& getBlock(const unsigned char& blockID);

};

#endif // BLOCK_H
