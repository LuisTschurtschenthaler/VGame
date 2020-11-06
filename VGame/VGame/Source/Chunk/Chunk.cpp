#include "TerrainGenerator.h"
#include "WorldConstants.h"
#include "ChunkManager.h"
#include "ChunkMesh.h"
#include "ChunkSection.h"
#include "NearbyChunks.h"
#include "Shader.h"
#include "Block.h"
#include "World.h"
#include "Random.h"
#include "Structure.h"
#include "ChunkMap.h"
#include "Util.h"


Chunk::Chunk(ChunkManager* chunkManager, ChunkCoordXZ coord)
	: chunkManager(chunkManager), coord(coord),
	chunkDataGenerated(false), meshGenerated(false), isBuffered(false) {

	for(int y = 0; y < CHUNK_SECTIONS; y++)
		_sections.push_back(new ChunkSection(chunkManager, this, { coord.x, y, coord.z }));

	for(int i = 0; i < AMOUNT_OF_MESH_TYPES; i++)
		meshCollection.push_back(new ChunkMesh(this));
}

Chunk::~Chunk() {
	/*
	chunkManager->chunks.erase(coord);
	chunkManager->chunkMaps.erase(coord);

	for(auto& section : _sections)
		section->~ChunkSection();

	for(auto& mesh : meshCollection)
		mesh->~ChunkMesh();
	*/
}


ChunkSection* Chunk::getChunkSection(int y) {
	if(y < 0 || y >= CHUNK_SECTIONS) return nullptr;
	return _sections[y];
}

void Chunk::placeBlock(const BlockPositionXYZ& coord, BlockType block) {
	BlockPositionXYZ bCoord = chunkManager->getBlockCoord(coord);
	ChunkCoordXZ cCoord = chunkManager->getChunkCoord(coord);
	
	getChunkSection(coord.y / CHUNK_SIZE)->placeBlock(bCoord, block);
}

void Chunk::generateChunkData(ChunkMap* chunkMap) {
	for(auto& section : _sections)
		chunkManager->terrainGenerator->generateBlockData(*section, chunkMap);

	generateFlora(chunkMap);
	chunkDataGenerated = true;
}

void Chunk::generateMesh() {
	chunkManager->getNearbyChunks(coord, nearbyChunks);
	
	for(auto& chunk : nearbyChunks) {
		if(!chunk->chunkDataGenerated)
			chunk->generateChunkData(chunkManager->getChunkMap({ chunk->coord.x, chunk->coord.z }));
	}

	for(auto& section : _sections)
		section->generateMesh();

	meshGenerated = true;
}

void Chunk::generateFlora(ChunkMap* chunkMap) {
	for(int x = 0; x < CHUNK_SIZE; x++) {
		for(int z = 0; z < CHUNK_SIZE; z++) {
			
			int height = chunkMap->heightMap.get(x, z);

			if(height > WATER_LEVEL + 2) {
				Biome* biome = chunkManager->terrainGenerator->getBiome(chunkMap, x, z);

				if(Random::isIntInRange(0, biome->getTreeFrequency()) == 5) {
					Structure structure;
					if(instanceof<Desert>(biome))
						structure.generateCactus({ x + coord.x * CHUNK_SIZE, height + 1, z + coord.z * CHUNK_SIZE });
					else
						structure.generateTree({ x + coord.x * CHUNK_SIZE, height + 1, z + coord.z * CHUNK_SIZE });

					structure.build(*chunkManager);
				}
			}
		}
	}
}

void Chunk::prepareDraw(int meshtype) {
	meshCollection[meshtype]->prepareDraw();
	isBuffered = true;
}

void Chunk::draw(int meshtype) {
	glBindBuffer(GL_ARRAY_BUFFER, meshCollection[meshtype]->getVBO());

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, uvWithTextureID));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshCollection[meshtype]->getIBO());
	glDrawElements(GL_TRIANGLES, meshCollection[meshtype]->indices.size(), GL_UNSIGNED_INT, (void*) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Chunk::save() {

}

int Chunk::getWorldPositionX(int x) const {
	return (x + coord.x * CHUNK_SIZE);
}

int Chunk::getWorldPositionZ(int z) const {
	return (z + coord.z * CHUNK_SIZE);
}


float Chunk::_vertexAO(bool side1, bool side2, bool corner) {
	return (side1 && side2) ? 0.f : static_cast<float>(3 - (side1 + side2 + corner));
}
