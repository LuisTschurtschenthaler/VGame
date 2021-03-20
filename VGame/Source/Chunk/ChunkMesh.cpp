#include <GLEW/GL/glew.h>
#include "ChunkMesh.h"
#include "Constants.h"
#include "Chunk.h"
#include "Game.h"
#include "ChunkManager.h"
#include "World.h"


unsigned int ChunkMesh::amountOfVertices = 0;
unsigned int ChunkMesh::amountOfIndices = 0;

const MeshFace ChunkMesh::_FACES[8] = {
	{ { 1, 0, 0,  1, 0, 1,  1, 1, 1,  1, 1, 0 },  { 3, 2, 0, 1 }, 0, 3 },
	{ { 0, 0, 1,  0, 0, 0,  0, 1, 0,  0, 1, 1 },  { 3, 2, 0, 1 }, 1, 3 },
	{ { 1, 1, 0,  1, 1, 1,  0, 1, 1,  0, 1, 0 },  { 1, 0, 2, 3 }, 2, 5 },
	{ { 0, 0, 0,  0, 0, 1,  1, 0, 1,  1, 0, 0 },  { 3, 2, 0, 1 }, 3, 2 },
	{ { 1, 0, 1,  0, 0, 1,  0, 1, 1,  1, 1, 1 },  { 3, 2, 0, 1 }, 4, 4 },
	{ { 1, 1, 0,  0, 1, 0,  0, 0, 0,  1, 0, 0 },  { 0, 1, 3, 2 }, 5, 4 },

	{ { 1, 1, 1,  1, 0, 1,  0, 0, 0,  0, 1, 0 },  { 0, 2, 3, 1 }, 0, 5 },
	{ { 1, 1, 0,  1, 0, 0,  0, 0, 1,  0, 1, 1 },  { 1, 3, 2, 0 }, 0, 5 }
};

const LocationXYZ ChunkMesh::_ADJACENTS[6] = {
	{  1,  0,  0 },
	{ -1,  0,  0 },
	{  0,  1,  0 },
	{  0, -1,  0 },
	{  0,  0,  1 },
	{  0,  0, -1 }
};


ChunkMesh::ChunkMesh(Chunk* chunk)
	: _chunk(chunk), _isBuffered(false) {

	vertices.reserve(CHUNK_AREA * 8);
	indices.reserve(CHUNK_AREA * 36);
}

ChunkMesh::~ChunkMesh() {
	_chunk = nullptr;
	clear();
}


void ChunkMesh::clear() {
	_isBuffered = false;

	amountOfIndices -= indices.size();
	amountOfVertices -= vertices.size();

	indices.clear();
	indices.shrink_to_fit();

	vertices.clear();
	vertices.shrink_to_fit();

	if(_VBO) glDeleteBuffers(1, &_VBO);
	if(_IBO) glDeleteBuffers(1, &_IBO);
}

void ChunkMesh::draw() {
	if(indices.size() <= 0 || vertices.size() <= 0)
		return;

	if(!_isBuffered) {
		glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ChunkVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		_isBuffered = true;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof(ChunkVertex), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*) 0);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ChunkMesh::addBlock(const Block& block, const unsigned char& x, const unsigned char& y, const unsigned char& z) {
	const LocationXYZ& blockLocation = { x, y, z };

	for(int i = 0; i < TOTAL_BLOCK_FACES; i++) {
		const Block& relativeBlock = _chunk->getBlockRelative(blockLocation + _ADJACENTS[i]);

		if(!relativeBlock.hasHitbox
		   || relativeBlock.isFloraBlock
		   || (!block.isTransparent && relativeBlock.isTransparent)) {

			_addBlockFace(block, i, x, y, z);
		}
	}
}

void ChunkMesh::addFluidBlock(const Block& block, const unsigned char& x, const unsigned char& y, const unsigned char& z) {
	const LocationXYZ& blockLocation = { x, y, z };

	for(int i = 0; i < TOTAL_BLOCK_FACES; i++) {
		const Block& relativeBlock = _chunk->getBlockRelative(blockLocation + _ADJACENTS[i]);

		if(relativeBlock.meshType == MeshType::NONE)
			_addBlockFace(block, i, x, y, z);
	}
}

void ChunkMesh::addFloraBlock(const Block& block, const unsigned char& x, const unsigned char& y, const unsigned char& z, 
							  const bool& isTall, const BlockFace& blockFace) {
	amountOfVertices += 8;
	amountOfIndices += 24;

	for(int i = 6; i <= 7; i++) {
		size_t indicesCount = vertices.size();
		indices.insert(indices.end(), {
			/* Clockwise */
			/* Triangle 1 */
			indicesCount + 0,
			indicesCount + 1,
			indicesCount + 2,

			/* Triangle 2 */
			indicesCount + 0,
			indicesCount + 2,
			indicesCount + 3,

			/* Counterclockwise */
			/* Triangle 1 */
			indicesCount + 2,
			indicesCount + 1,
			indicesCount + 0,

			/* Triangle 2 */
			indicesCount + 3,
			indicesCount + 2,
			indicesCount + 0
		});
		
		const MeshFace& meshFace = _FACES[i];

		int index = 0;
		for(int j = 0; j < 4; j++) {
			unsigned char xi = x + meshFace.vertices[index++],
						  yi = y + meshFace.vertices[index++],
						  zi = z + meshFace.vertices[index++];

			ChunkVertex chunkVertex = {
				(xi | yi << 8 | zi << 16 | meshFace.lightLevel << 24),
				(block.textures[(int) blockFace] | meshFace.textureCoords[j] << 8 | meshFace.normal << 16)
			};

			vertices.push_back(chunkVertex);
		}
	}
	if(isTall) addFloraBlock(block, x, y + 1, z, false, BlockFace::FACE_TOP);
}



void ChunkMesh::_addBlockFace(const Block& block, const int& face, const unsigned char& x, const unsigned char& y, const unsigned char& z) {
	amountOfVertices += 4;
	amountOfIndices += 6;

	size_t indicesCount = vertices.size();
	indices.insert(indices.end(), {
		/* Triangle 1 */
		indicesCount + 0,
		indicesCount + 1,
		indicesCount + 2,

		/* Triangle 2 */
		indicesCount + 0,
		indicesCount + 2,
		indicesCount + 3 
	});

	const MeshFace& meshFace = _FACES[(int) face];

	int index = 0;
	for(int i = 0; i < 4; i++) {
		unsigned char xi = x + meshFace.vertices[index++],
					  yi = y + meshFace.vertices[index++],
					  zi = z + meshFace.vertices[index++];

		ChunkVertex chunkVertex = {
			(xi | yi << 8 | zi << 16 | meshFace.lightLevel << 24),
			(block.textures[(int) face] | meshFace.textureCoords[i] << 8 | meshFace.normal << 16)
		};

		vertices.push_back(chunkVertex);
	}
}

unsigned char ChunkMesh::_getVertexAO(const bool& corner, const bool& side1, const bool& side2) {
	return (side1 && side2) ? 3 : (side1 + side2 + corner);
}
