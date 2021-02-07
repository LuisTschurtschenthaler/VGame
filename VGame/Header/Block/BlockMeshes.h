#ifndef BLOCK_MESHES_H
#define BLOCK_MESHES_H

#include <vector>

#include "Vertex.h"


struct BlockMesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};


enum class BlockMeshType {
	BLOCK,
	HALF_SLAP_TOP,
	HALF_SLAP_BOTTOM,
};

#endif // BLOCK_MESHES_H
