#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <map>
#include <vector>
#include <array>
#include "Vertex.h"
#include "Block.h"
#include "Orientation.h"
#include "MeshTypes.h"
#include "Coordinates.h"

class Chunk;


struct MeshFace {
	std::array<unsigned char, 12> vertices;
	std::array<unsigned char, 4> textureCoords;
	unsigned char normal, lightLevel;
};

struct VertexAO {
	std::array<unsigned char, 3> adjacents;
};


class ChunkMesh {

public:
	static unsigned int amountOfVertices, amountOfIndices;

private:
	static const MeshFace _FACES[8];
	static const LocationXYZ _ADJACENTS[6], _AO_ADJACENTS[16];
	static const VertexAO _AO[6][4*2];

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

	void addBlock(const Block& block, const unsigned char& x, const unsigned char& y, const unsigned char& z);
	void addFluidBlock(const Block& block, const unsigned char& x, const unsigned char& y, const unsigned char& z);
	void addFloraBlock(const Block& block, const unsigned char& x, const unsigned char& y, const unsigned char& z, const bool& isTall = false, const BlockFace& blockFace = BlockFace::FACE_BOTTOM);

private:
	void _addBlockFace(const Block& block, const int& face, const unsigned char& x, const unsigned char& y, const unsigned char& z);
	unsigned char _vertexAO(const LocationXYZ& location, const int& face, const int& vertex);

};

#endif // CHUNK_MESH_H
