#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <GLEW/GL/glew.h>
#include <map>
#include <vector>
#include "Vertex.h"
#include "Block.h"
#include "WorldConstants.h"
#include "MeshTypes.h"
#include "Shader.h"

class Chunk;
class World;
class Game;
class ChunkSection;


class ChunkMesh {

public:
	static int amountOfVertices, amountOfIndices;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

private:
	Chunk* _chunk;
	GLuint _VBO, _IBO;
	bool _isBuffered;


public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();

	void prepareDraw();
	void draw();

	void addBlockFace(const ChunkSection* chunkSection, int xi, int yi, int zi, const BlockFace face, Block* block);
	void addFloraBlock(const ChunkSection* chunkSection, int x, int y, int z, const BlockFace face, Block* block);

	GLuint getVBO() const { return _VBO; }
	GLuint getIBO() const { return _IBO; }

private:
	float _vertexAO(bool corner, bool side1, bool side2);

};

#endif // CHUNK_MESH_H
