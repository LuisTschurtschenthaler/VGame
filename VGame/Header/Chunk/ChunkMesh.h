#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <map>
#include <vector>
#include "Vertex.h"
#include "Block.h"
#include "Orientation.h"
#include "MeshTypes.h"

class Chunk;


class ChunkMesh {

public:
	static int amountOfVertices, amountOfIndices;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

private:
	Chunk* _chunk;
	unsigned int _VBO, _IBO;
	bool _isBuffered;


public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();

	void draw();
	void clear();

	void addBlock(const Chunk* chunk, const Block* block, const int& x, const int& y, const int& z);
	void addBlockFace(const Chunk* chunk, const Block* block, const BlockFace& face, const int& x, const int& y, const int& z);
	void addFluidBlock(const Chunk* chunk, const Block* block, const int& x, const int& y, const int& z);
	void addFloraBlock(const Chunk* chunk, const Block* block, const BlockFace& face, const int& x, const int& y, const int& z);

private:
	float _vertexAO(bool corner, bool side1, bool side2);

};

#endif // CHUNK_MESH_H
