#include <GLEW/GL/glew.h>
#include "ChunkMesh.h"
#include "Constants.h"
#include "Chunk.h"
#include "Game.h"
#include "ChunkManager.h"
#include "World.h"


int ChunkMesh::amountOfVertices = 0;
int ChunkMesh::amountOfIndices = 0;

const MeshFace ChunkMesh::_faces[8] = {
	{ { 1, 0, 0,  1, 0, 1,  1, 1, 1,  1, 1, 0 },  { 3, 2, 0, 1 },  3 },
	{ { 0, 0, 1,  0, 0, 0,  0, 1, 0,  0, 1, 1 },  { 3, 2, 0, 1 },  3 },
	{ { 1, 1, 0,  1, 1, 1,  0, 1, 1,  0, 1, 0 },  { 1, 0, 2, 3 },  5 },
	{ { 0, 0, 0,  0, 0, 1,  1, 0, 1,  1, 0, 0 },  { 3, 2, 0, 1 },  2 },
	{ { 1, 0, 1,  0, 0, 1,  0, 1, 1,  1, 1, 1 },  { 3, 2, 0, 1 },  4 },
	{ { 1, 1, 0,  0, 1, 0,  0, 0, 0,  1, 0, 0 },  { 0, 1, 3, 2 },  4 },

	{ { 1, 1, 1,  1, 0, 1,  0, 0, 0,  0, 1, 0 },  { 0, 2, 3, 1 },  5 },
	{ { 1, 1, 0,  1, 0, 0,  0, 0, 1,  0, 1, 1 },  { 1, 3, 2, 0 },  5 }
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

void ChunkMesh::addBlock(const Block& block, const uint8_t& x, const uint8_t& y, const uint8_t& z) {
	const std::vector<LocationXYZ> ADJACENTS = {
		{  1,  0,  0 },
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{  0, -1,  0 },
		{  0,  0,  1 },
		{  0,  0, -1 }
	};

	for(int i = 0; i < TOTAL_BLOCK_FACES; i++) {
		const Block& relativeBlock = _chunk->getBlockRelative(LocationXYZ(x, y, z) + ADJACENTS[i]);

		if(!relativeBlock.hasHitbox
		   || relativeBlock.isFloraBlock
		   || (!block.isTransparent && (relativeBlock.isTransparent || relativeBlock.isFloraBlock))) {

			addBlockFace(block, static_cast<BlockFace>(i), x, y, z);
		}
	}
}

void ChunkMesh::addBlockFace(const Block& block, const BlockFace& face, const uint8_t& x, const uint8_t& y, const uint8_t& z) {
	amountOfVertices += 4;
	amountOfIndices += 6;

	size_t indicesCount = vertices.size();
	indices.insert(indices.end(), {
		/* Triangle 1 */
		indicesCount + 0,	/*    /|0 */
		indicesCount + 1,	/*   / |  */
		indicesCount + 2,	/* 2/__|1 */

		/* Triangle 2          3____0 */
		indicesCount + 0,  /*  |  /  */
		indicesCount + 2,  /*  | /   */
		indicesCount + 3   /* 2|/    */
	});

	const MeshFace& meshFace = _faces[(int) face];

	int index = 0;
	for(unsigned int i = 0; i < 4; i++) {
		uint8_t xi = x + meshFace.vertices[index++],
				yi = y + meshFace.vertices[index++],
				zi = z + meshFace.vertices[index++];

		ChunkVertex chunkVertex = {
			(xi | yi << 8 | zi << 16 | meshFace.lightLevel << 24),
			(block.textures[(int) face] | meshFace.textureCoords[i] << 8)
		};

		vertices.push_back(chunkVertex);
	}
}

void ChunkMesh::addFluidBlock(const Block& block, const uint8_t& x, const uint8_t& y, const uint8_t& z) {
	const std::vector<LocationXYZ> ADJACENTS = {
		{  1,  0,  0 },
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{  0, -1,  0 },
		{  0,  0,  1 },
		{  0,  0, -1 }
	};

	for(int i = 0; i < TOTAL_BLOCK_FACES; i++) {
		const Block& relativeBlock = _chunk->getBlockRelative(LocationXYZ(x, y, z) + ADJACENTS[i]);

		if(relativeBlock.meshType == NONE)
			addBlockFace(block, static_cast<BlockFace>(i), x, y, z);
	}
}

void ChunkMesh::addFloraBlock(const Block& block, const uint8_t& x, const uint8_t& y, const uint8_t& z) {
	//amountOfVertices += 8;
	//amountOfIndices += 24;

	for(int i = 6; i <= 7; i++) {
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

		const MeshFace& meshFace = _faces[i];

		int index = 0;
		for(unsigned int j = 0; j < 4; j++) {
			uint8_t xi = x + meshFace.vertices[index++],
					yi = y + meshFace.vertices[index++],
					zi = z + meshFace.vertices[index++];

			ChunkVertex chunkVertex = {
				(xi | yi << 8 | zi << 16 | meshFace.lightLevel << 24),
				(block.textures[BlockFace::FACE_FRONT] | meshFace.textureCoords[j] << 8)
			};

			vertices.push_back(chunkVertex);
		}
	}
	/**amountOfVertices += 8;
	amountOfIndices += 24;
	
	int textureID = block.textures[face];
	Vertex v1(
		glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 0.f), textureID);
	Vertex v2(
		glm::vec3(x + BLOCK_SIZE, y, z + BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 1.f), textureID);
	Vertex v3(
		glm::vec3(x, y, z),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(1.f, 1.f), textureID);
	Vertex v4(
		glm::vec3(x, y + BLOCK_SIZE, z),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(1.f, 0.f), textureID);

	Vertex v5(
		glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(1.f, 0.f), textureID);
	Vertex v6(
		glm::vec3(x + BLOCK_SIZE, y, z),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(1.f, 1.f), textureID);
	Vertex v7(
		glm::vec3(x, y, z + BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 1.f), textureID);
	Vertex v8(
		glm::vec3(x, y + BLOCK_SIZE, z + BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 0.f), textureID);

	size_t index = vertices.size();
	indices.insert(indices.end(), {
		/* Face 1 clockwise 
		index + 0,
		index + 1,
		index + 2,
		index + 0,
		index + 2,
		index + 3,

		/* Face 2 clockwise
		index + 6,
		index + 5,
		index + 4,
		index + 7,
		index + 6,
		index + 4,

		/* Face 1 counterclockwise 
		index + 2,
		index + 1,
		index + 0,
		index + 3,
		index + 2,
		index + 0,

		/* Face 2 counterclockwise 
		index + 4,
		index + 5,
		index + 6,
		index + 4,
		index + 6,
		index + 7
	});

	vertices.insert(vertices.end(), {
		v1, v2, v3, v4,
		v5, v6, v7, v8
	});
	*/
}

uint8_t ChunkMesh::_vertexAO(const bool& corner, const bool& side1, const bool& side2) {
	return (side1 && side2) ? 3 : (side1 + side2 + corner);
}
