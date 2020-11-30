#include "ChunkMesh.h"
#include "WorldConstants.h"
#include "Shader.h"
#include "Chunk.h"
#include "ChunkSection.h"


std::map<MeshType, Shader*> ChunkMesh::meshShaders;
int ChunkMesh::amountOfVertices = 0;
int ChunkMesh::amountOfIndices = 0;


ChunkMesh::ChunkMesh(Chunk* chunk)
	: _chunk(chunk), _isBuffered(false) {

}

ChunkMesh::~ChunkMesh() {
	indices.clear();
	indices.shrink_to_fit();

	vertices.clear();
	vertices.shrink_to_fit();

	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_IBO);
}


void ChunkMesh::prepareDraw() {
	if(!_isBuffered) {
		glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),&vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
		_isBuffered = true;
	}
}

void ChunkMesh::draw() {
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ChunkMesh::addBlockFace(const ChunkSection* chunkSection, int xi, int yi, int zi, const BlockFace face, Block* block) {
	float x = (xi + chunkSection->coord.x * CHUNK_SIZE) * BLOCK_SIZE;
	float y = (yi + chunkSection->coord.y * CHUNK_SIZE) * BLOCK_SIZE;
	float z = (zi + chunkSection->coord.z * CHUNK_SIZE) * BLOCK_SIZE;
	float txOffset = block->texturePixelOffset / 16;

	amountOfVertices += 4;
	amountOfIndices += 6;
	
	// OPIMISATION WITH CUSTOM DATA TYPES
	// https://en.cppreference.com/w/cpp/types/integer

	int textureID = 0;
	Vertex v1, v2, v3, v4;
	switch(face) {
		case FACE_RIGHT: textureID = block->textures[FACE_RIGHT];
			v1 = Vertex(
				glm::vec3(x + BLOCK_SIZE - txOffset, y, z - BLOCK_SIZE),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(1.f, 1.f), textureID);
			v2 = Vertex(
				glm::vec3(x + BLOCK_SIZE - txOffset, y + BLOCK_SIZE, z - BLOCK_SIZE),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(1.f, 0.f), textureID);
			v3 = Vertex(
				glm::vec3(x + BLOCK_SIZE - txOffset, y, z),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(0.f, 1.f), textureID);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE - txOffset, y + BLOCK_SIZE, z),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec2(0.f, 0.f), textureID);
			break;

		case FACE_LEFT: textureID = block->textures[FACE_LEFT];
			v1 = Vertex(
				glm::vec3(x + txOffset, y, z - BLOCK_SIZE),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(1.f, 1.f), textureID);
			v2 = Vertex(
				glm::vec3(x + txOffset, y, z),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(0.f, 1.f), textureID);
			v3 = Vertex(
				glm::vec3(x + txOffset, y + BLOCK_SIZE, z - BLOCK_SIZE),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(1.f, 0.f), textureID);
			v4 = Vertex(
				glm::vec3(x + txOffset, y + BLOCK_SIZE, z),
				glm::vec3(-1.f, 0.f, 0.f),
				glm::vec2(0.f, 0.f), textureID);
			break;

		case FACE_TOP: textureID = block->textures[FACE_TOP];
			v1 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE, z - BLOCK_SIZE),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(1.f, 1.f), textureID);
			v2 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE, z),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(1.f, 0.f), textureID);
			v3 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(0.f, 1.f), textureID);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z),
				glm::vec3(0.f, 1.f, 0.f),
				glm::vec2(0.f, 0.f), textureID);
			break;

		case FACE_BOTTOM: textureID = block->textures[FACE_BOTTOM];
			v1 = Vertex(
				glm::vec3(x, y, z - BLOCK_SIZE),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(1.f, 1.f), textureID);
			v2 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y, z - BLOCK_SIZE),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(1.f, 0.f), textureID);
			v3 = Vertex(
				glm::vec3(x, y, z),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(0.f, 1.f), textureID);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y, z),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec2(0.f, 0.f), textureID);
			break;

		case FACE_FRONT: textureID = block->textures[FACE_FRONT];
			v1 = Vertex(
				glm::vec3(x, y, z - txOffset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(1.f, 1.f), textureID);
			v2 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y, z - txOffset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(0.f, 1.f), textureID);
			v3 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE, z - txOffset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(1.f, 0.f), textureID);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - txOffset),
				glm::vec3(0.f, 0.f, 1.f),
				glm::vec2(0.f, 0.f), textureID);
			break;

		case FACE_BACK: textureID = block->textures[FACE_BACK];
			v1 = Vertex(
				glm::vec3(x, y, z - BLOCK_SIZE + txOffset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(1.f, 1.f), textureID);
			v2 = Vertex(
				glm::vec3(x, y + BLOCK_SIZE, z - BLOCK_SIZE + txOffset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(1.f, 0.f), textureID);
			v3 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y, z - BLOCK_SIZE + txOffset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(0.f, 1.f), textureID);
			v4 = Vertex(
				glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE + txOffset),
				glm::vec3(0.f, 0.f, -1.f),
				glm::vec2(0.f, 0.f), textureID);
			break;
	}
	
	size_t index = vertices.size();
	indices.insert(indices.end(), {
		/* Triangle 1 */
		index,
		index + 1,
		index + 2,

		/* Triangle 2 */
		index + 3,
		index + 2,
		index + 1
	});

	vertices.insert(vertices.end(), {
		v1, v2, v3, v4
	});
}

void ChunkMesh::addFloraBlock(const ChunkSection* chunkSection, int xi, int yi, int zi, const BlockFace face, Block* block) {
	float x = (xi + chunkSection->coord.x * CHUNK_SIZE) * BLOCK_SIZE;
	float y = (yi + chunkSection->coord.y * CHUNK_SIZE) * BLOCK_SIZE;
	float z = (zi + chunkSection->coord.z * CHUNK_SIZE) * BLOCK_SIZE;

	int textureID = block->textures[face];
	Vertex v1(
		glm::vec3(x, y, z),
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec2(1.f, 1.f), textureID);
	Vertex v2(
		glm::vec3(x + BLOCK_SIZE, y, z - BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 1.f), textureID);
	Vertex v3(
		glm::vec3(x, y + BLOCK_SIZE, z),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(1.f, 0.f), textureID);
	Vertex v4(
		glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z - BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 0.f), textureID);

	Vertex v5(
		glm::vec3(x + BLOCK_SIZE, y, z),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(1.f, 1.f), textureID);
	Vertex v6(
		glm::vec3(x, y, z - BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 1.f), textureID);
	Vertex v7(
		glm::vec3(x + BLOCK_SIZE, y + BLOCK_SIZE, z),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(1.f, 0.f), textureID);
	Vertex v8(
		glm::vec3(x, y + BLOCK_SIZE, z - BLOCK_SIZE),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec2(0.f, 0.f), textureID);

	size_t index = vertices.size();
	indices.insert(indices.end(), {
		/* Triangle 1 */
		index,
		index + 1,
		index + 2,

		/* Triangle 2 */
		index + 3,
		index + 2,
		index + 1,

		/* Triangle 3 */
		index + 4,
		index + 5,
		index + 6,

		/* Triangle 4 */
		index + 7,
		index + 6,
		index + 5
	});

	vertices.insert(vertices.end(), {
		v1, v2, v3, v4,
		v5, v6, v7, v8
	});
}
