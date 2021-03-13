#include <GLEW/GL/glew.h>
#include "ChunkMesh.h"
#include "Constants.h"
#include "Chunk.h"
#include "Game.h"
#include "ChunkManager.h"
#include "World.h"


int ChunkMesh::amountOfVertices = 0;
int ChunkMesh::amountOfIndices = 0;


ChunkMesh::ChunkMesh(Chunk* chunk)
	: _chunk(chunk), _isBuffered(false) {

	vertices.reserve(CHUNK_AREA * 8);
	indices.reserve(CHUNK_AREA * 36);
}

ChunkMesh::~ChunkMesh() {
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		_isBuffered = true;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, ambientOcclusion));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ChunkMesh::addBlock(const Chunk* chunk, const Block& block, const int& x, const int& y, const int& z) {
	const std::vector<LocationXYZ> ADJACENTS = {
		{  1,  0,  0 },
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{  0, -1,  0 },
		{  0,  0,  1 },
		{  0,  0, -1 }
	};

	for(int i = 0; i < TOTAL_BLOCK_FACES; i++) {
		const Block& relativeBlock = chunk->getBlockRelative(LocationXYZ(x, y, z) + ADJACENTS[i]);

		if(!relativeBlock.hasHitbox
		   || relativeBlock.isFloraBlock
		   || (!block.isTransparent && (relativeBlock.isTransparent || relativeBlock.isFloraBlock))) {

			addBlockFace(chunk, block, static_cast<BlockFace>(i), x, y, z);
		}
	}
}

void ChunkMesh::addBlockFace(const Chunk* chunk, const Block& block, const BlockFace& face, const int& xi, const int& y, const int& zi) {
	int x = xi + chunk->coord.x * CHUNK_SIZE;
	int z = zi + chunk->coord.z * CHUNK_SIZE;

	amountOfVertices += 4;
	amountOfIndices += 6;

	int textureID = 0;
	float offset = 0;

	Vertex v1, v2, v3, v4;
	switch(face) {
		case FACE_RIGHT:
			textureID = block.textures[FACE_RIGHT];
			offset = block.texturePixelOffset[FACE_RIGHT] / 16.f;

			v1 = Vertex(
				glm::vec3(x + BLOCK_SIZE - offset, y, z),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(1.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y - 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi - 1).useAmbient
				) : 0.f
			);
			v2 = Vertex(
				glm::vec3(x + BLOCK_SIZE - offset, y, z + BLOCK_SIZE),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(0.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y - 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi + 1).useAmbient
				) : 0.f
			);
			v3 = Vertex(
				glm::vec3(x + BLOCK_SIZE - offset, y + BLOCK_SIZE, z + BLOCK_SIZE),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(0.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y + 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi + 1).useAmbient
				) : 0.f
			);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE - offset, y + BLOCK_SIZE, z),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(1.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y + 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi - 1).useAmbient
				) : 0.f
			);
			break;

		case FACE_LEFT:
			textureID = block.textures[FACE_LEFT];
			offset = block.texturePixelOffset[FACE_LEFT] / 16.f;

			v1 = Vertex(
				glm::vec3(x + offset, y, z + BLOCK_SIZE),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(1.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y - 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi + 1).useAmbient
				) : 0.f
			);
			v2 = Vertex(
				glm::vec3(x + offset, y, z),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(0.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y - 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi - 1).useAmbient
				) : 0.f
			);
			v3 = Vertex(
				glm::vec3(x + offset, y + BLOCK_SIZE, z),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(0.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y + 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi - 1).useAmbient
				) : 0.f
			);
			v4 = Vertex(
				glm::vec3(x + offset, y + BLOCK_SIZE, z + BLOCK_SIZE),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(1.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y + 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi + 1).useAmbient
				) : 0.f
			);
			break;

		case FACE_TOP:
			textureID = block.textures[FACE_TOP];
			offset = block.texturePixelOffset[FACE_TOP] / 16.f;

			v1 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE - offset, z),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(1.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y + 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y + 1, zi).useAmbient
				) : 0.f
			);
			v2 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE - offset, z + BLOCK_SIZE),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(0.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y + 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi + 1).useAmbient
				) : 0.f
			);
			v3 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE - offset, z + BLOCK_SIZE),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(0.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y + 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi + 1).useAmbient
				) : 0.f
			);
			v4 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE - offset, z),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(1.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y + 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi - 1).useAmbient
				) : 0.f
			);
			break;

		case FACE_BOTTOM:
			textureID = block.textures[FACE_BOTTOM];
			offset = block.texturePixelOffset[FACE_BOTTOM] / 16.f;

			v1 = Vertex(
				glm::vec3(x, y + offset, z),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(1.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y - 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi - 1).useAmbient
				) : 0.f
			);
			v2 = Vertex(
				glm::vec3(x, y + offset, z + BLOCK_SIZE),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(0.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y - 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi + 1).useAmbient
				) : 0.f
			);
			v3 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + offset, z + BLOCK_SIZE),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(0.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y - 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi + 1).useAmbient
				) : 0.f
			);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + offset, z),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(1.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y - 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi - 1).useAmbient
				) : 0.f
			);
			break;

		case FACE_FRONT:
			textureID = block.textures[FACE_FRONT];
			offset = block.texturePixelOffset[FACE_FRONT] / 16.f;

			v1 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y, z + BLOCK_SIZE - offset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(1.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y - 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi + 1).useAmbient
				) : 0.f
			);
			v2 = Vertex(
				glm::vec3(x, y, z + BLOCK_SIZE - offset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(0.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y - 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi + 1).useAmbient
				) : 0.f
			);
			v3 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE, z + BLOCK_SIZE - offset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(0.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y + 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi + 1).useAmbient
				) : 0.f
			);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE - offset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(1.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y + 1, zi + 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi + 1).useAmbient
				) : 0.f
			);
			break;

		case FACE_BACK:
			textureID = block.textures[FACE_BACK];
			offset = block.texturePixelOffset[FACE_BACK] / 16.f;

			v1 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z + offset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(0.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
				chunk->getBlockRelative(xi + 1, y + 1, zi - 1).useAmbient,
				chunk->getBlockRelative(xi + 1, y + 1, zi).useAmbient,
				chunk->getBlockRelative(xi, y + 1, zi - 1).useAmbient
			) : 0.f
			);
			v2 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE, z + offset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(1.f, 0.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y + 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y + 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y + 1, zi - 1).useAmbient
				) : 0.f
			);
			v3 = Vertex(
				glm::vec3(x, y, z + offset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(1.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi - 1, y - 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi - 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi - 1).useAmbient
				) : 0.f
			);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y, z + offset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(0.f, 1.f), textureID,
				block.useAmbient ? _vertexAO(
					chunk->getBlockRelative(xi + 1, y - 1, zi - 1).useAmbient,
					chunk->getBlockRelative(xi + 1, y - 1, zi).useAmbient,
					chunk->getBlockRelative(xi, y - 1, zi - 1).useAmbient
				) : 0.f
			);
			break;
	}

	size_t index = vertices.size();
	indices.insert(indices.end(), {
		/* Triangle 1 */
		index + 0,	/*    /|0 */
		index + 1,	/*   / |  */
		index + 2,	/* 2/__|1 */

		/* Triangle 2  3____0 */
		index + 0,  /*  |  /  */
		index + 2,  /*  | /   */
		index + 3   /* 2|/    */
	});

	vertices.insert(vertices.end(), {
		v1, v2, v3, v4
	});
}

void ChunkMesh::addFluidBlock(const Chunk* chunk, const Block& block, const int& x, const int& y, const int& z) {
	const std::vector<LocationXYZ> ADJACENTS = {
		{  1,  0,  0 },
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{  0, -1,  0 },
		{  0,  0,  1 },
		{  0,  0, -1 }
	};

	for(int i = 0; i < TOTAL_BLOCK_FACES; i++) {
		const Block& relativeBlock = chunk->getBlockRelative(LocationXYZ(x, y, z) + ADJACENTS[i]);

		if(relativeBlock.meshType == NONE)
			addBlockFace(chunk, block, static_cast<BlockFace>(i), x, y, z);
	}
}

void ChunkMesh::addFloraBlock(const Chunk* chunk, const Block& block, const BlockFace& face, const int& xi, const int& y, const int& zi) {
	float x = xi + chunk->coord.x * CHUNK_SIZE;
	float z = zi + chunk->coord.z * CHUNK_SIZE;

	amountOfVertices += 8;
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
		/* Face 1 clockwise */
		index + 0,
		index + 1,
		index + 2,
		index + 0,
		index + 2,
		index + 3,

		/* Face 2 clockwise */
		index + 6,
		index + 5,
		index + 4,
		index + 7,
		index + 6,
		index + 4,

		/* Face 1 counterclockwise */
		index + 2,
		index + 1,
		index + 0,
		index + 3,
		index + 2,
		index + 0,

		/* Face 2 counterclockwise */
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
}

float ChunkMesh::_vertexAO(const bool& corner, const bool& side1, const bool& side2) {
	return (side1 && side2) ? 3.f : float(side1 + side2 + corner);
}
