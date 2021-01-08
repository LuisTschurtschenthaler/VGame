#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <map>
#include <vector>
#include "Vertex.h"
#include "Block.h"
#include "Orientation.h"
#include "MeshTypes.h"

class Chunk;
class Player;


class ChunkMesh {

public:
	static int amountOfVertices, amountOfIndices;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

private:
	Chunk* _chunk;
	Player* _player;
	unsigned int _VBO, _IBO;
	bool _isBuffered;


public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();

	void draw();
	void clear();

	void addBlockFace(const Chunk* chunk, int x, int y, int z, const BlockFace face, Block* block);
	void addFloraBlock(const Chunk* chunk, int x, int y, int z, const BlockFace face, Block* block);

private:
	float _vertexAO(bool corner, bool side1, bool side2);

};

#endif // CHUNK_MESH_H
