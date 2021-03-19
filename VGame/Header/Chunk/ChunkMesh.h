#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <map>
#include <vector>
#include <array>
#include "Vertex.h"
#include "Block.h"
#include "Orientation.h"
#include "MeshTypes.h"

class Chunk;


struct MeshFace {
	std::array<uint8_t, 12> vertices;
	std::array<uint8_t, 4> textureCoords;
	uint8_t lightLevel;
};




class ChunkMesh {

public:
	static int amountOfVertices, amountOfIndices;

private:
	static const MeshFace _faces[8];

public:
	std::vector<ChunkVertex> vertices;
	std::vector<unsigned int> indices;

private:
	Chunk* _chunk;
	unsigned int _VBO, _IBO;
	bool _isBuffered;


public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();

	void clear();
	void draw();

	void addBlock(const Block& block, const uint8_t& x, const uint8_t& y, const uint8_t& z);
	void addBlockFace(const Block& block, const BlockFace& face, const uint8_t& x, const uint8_t& y, const uint8_t& z);
	void addFluidBlock(const Block& block, const uint8_t& x, const uint8_t& y, const uint8_t& z);
	void addFloraBlock(const Block& block, const uint8_t& x, const uint8_t& y, const uint8_t& z);

private:
	uint8_t _vertexAO(const bool& corner, const bool& side1, const bool& side2);

};

#endif // CHUNK_MESH_H
