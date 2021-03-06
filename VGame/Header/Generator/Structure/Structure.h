#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <vector>
#include <GLM/glm.hpp>
#include "Coordinates.h"
#include "BlockID.h"

class ChunkManager;


struct SturctureBlock {
	LocationXYZ position;
	BlockID block;

	SturctureBlock(int x, int y, int z, BlockID block)
		: position({ x, y, z }), block(block) { }
};


class Structure {

private:
	std::vector<SturctureBlock> _structureBlocks;

public:
	static const int CROWN_RADIUS, BUSH_RADIUS;


public:
	void build();
	void generateTree(const LocationXYZ& pos, const BlockID& logBlock, const BlockID& leaveBlock);
	void generateCactus(const LocationXYZ& pos);

	void fillXZ(const LocationXYZ& start, const LocationXYZ& end, const BlockID& blockID);
	void fillY(const LocationXYZ& start, const BlockID& blockID, const int& height);
	void clearEdges(const LocationXYZ& pos, const int& distance);

};

#endif // STRUCTURE_H
