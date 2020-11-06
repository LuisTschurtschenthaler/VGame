#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <GLEW/GL/glew.h>
#include <map>
#include <vector>
#include "Vertex.h"
#include "Block.h"
#include "WorldConstants.h"
#include "MeshTypes.h"
class Chunk;
class Shader;
class World;
class ChunkSection;


class ChunkMesh {

public:
	static int amountOfVertices, amountOfIndices;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

private:
	Chunk* _chunk;
	GLuint _VBO, _IBO;


public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();

	void prepareDraw();
	void addBlockFace(const ChunkSection* chunkSection, int xi, int yi, int zi, const BlockFace face, Block* block);

	GLuint getVBO() const { return _VBO; }
	GLuint getIBO() const { return _IBO; }

};


#endif // CHUNK_MESH_H
