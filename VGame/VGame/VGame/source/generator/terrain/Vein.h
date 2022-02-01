#ifndef VEIN_H
#define VEIN_H

#include <vector>
#include "Coordinates.h"
#include "BlockID.h"

class ChunkArea;


struct VeinSettings {
	BlockID block;
	int spawnTries;
	int minSize, maxSize;
	int minHeight, maxHeight;
};


class Vein {

private:
	static const std::vector<LocationXYZ> _directions;


public:
	static void generate(ChunkArea* chunkArea, const VeinSettings& veinSettings);

};

#endif // VEIN_H
