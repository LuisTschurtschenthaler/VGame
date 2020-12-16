#include <GLM/glm.hpp>
#include "Chunk.h"
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
#include "AABB.h"
#include "World.h"


Chunk::Chunk(ChunkManager* chunkManager, ChunkCoordXZ coord)
	: chunkManager(chunkManager), coord(coord),
	chunkDataGenerated(false), meshGenerated(false) {
	
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

void Chunk::placeBlock(const BlockPositionXYZ& bcoord, BlockType block) {
	BlockPositionXYZ bCoord = chunkManager->getBlockCoord(bcoord);
	getChunkSection(bcoord.y / CHUNK_SIZE)->placeBlock(bCoord, block);
}

void Chunk::generateChunkData(ChunkMap* chunkMap) {
	generateFlora(chunkMap);

	for(auto& section : _sections)
		chunkManager->terrainGenerator->generateBlockData(*section, chunkMap);

	chunkDataGenerated = true;
}

void Chunk::generateMesh() {
	chunkManager->getNearbyChunks(coord, nearbyChunks);

	for(auto& chunk : nearbyChunks) {
		if(!chunk->chunkDataGenerated)
			chunk->generateChunkData(chunkManager->getChunkMap(chunk->coord));
	}

	for(auto& section : _sections)
		section->generateMesh();

	meshGenerated = true;
}

/* IMPROVE THIS SHIT */
void Chunk::generateFlora(ChunkMap* chunkMap) {
	for(int x = 0; x < CHUNK_SIZE; x++)
	for(int z = 0; z < CHUNK_SIZE; z++) {
		int height = chunkMap->heightMap.get(x, z);

		if(height > WATER_LEVEL + 2) {
			Biome* biome = chunkManager->terrainGenerator->getBiome(chunkMap, x, z);

			if(Random::isIntInRange(0, biome->getTreeFrequency()) == 5) {
				Structure structure;
				if(instanceof<Desert>(biome))
					structure.generateCactus({ x + coord.x * CHUNK_SIZE, height, z + coord.z * CHUNK_SIZE });
				else structure.generateTree({ x + coord.x * CHUNK_SIZE, height, z + coord.z * CHUNK_SIZE });
				
				structure.build(*chunkManager);
			}
			else if(Random::isIntInRange(0, biome->getPlantFrequency()) == 5)
				chunkManager->placeBlock({ x + coord.x * CHUNK_SIZE, height + 1, z + coord.z * CHUNK_SIZE }, biome->getPlant());
		}
	}
}

void Chunk::draw(int meshtype) {
	meshCollection[meshtype]->prepareDraw();
	meshCollection[meshtype]->draw();
}

void Chunk::save() {
}

int Chunk::getWorldPositionX(int x) const {
	return (x + coord.x * CHUNK_SIZE);
}

int Chunk::getWorldPositionZ(int z) const {
	return (z + coord.z * CHUNK_SIZE);
}


bool Chunk::_isOutOfRange(const BlockPositionXYZ& coord) {
	return ((coord.x >= CHUNK_SIZE || coord.x < 0) || (coord.y >= CHUNK_SIZE || coord.y < 0) || (coord.z >= CHUNK_SIZE || coord.z < 0));
}
