#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <GLM/glm.hpp>
#include <vector>
#include "Coordinates.h"
#include "BlockType.h"
class ChunkManager;


struct SturctureBlock {
	BlockPositionXYZ position;
	BlockType block;

	SturctureBlock(int x, int y, int z, BlockType block)
		: position({ x, y, z }), block(block) { }
};


class Structure {

private:
	std::vector<SturctureBlock> _structureBlocks;


public:
	void build(ChunkManager& chunkManager);
	void generateTree(const BlockPositionXYZ& pos);
	void generateCactus(const BlockPositionXYZ& pos);

	void fillXZ(const BlockPositionXYZ& start, const BlockPositionXYZ& end, BlockType block);
	void fillY(const BlockPositionXYZ& start, BlockType block, int height);
	void clearEdges(const BlockPositionXYZ& pos, int distance);

};

#endif // STRUCTURE_H
