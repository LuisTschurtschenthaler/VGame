#include "ChunkArea.h"
#include "World.h"
#include "ChunkManager.h"
#include "WorldGenerator.h"
#include "Structure.h"
#include "Util.h"


ChunkArea::ChunkArea(const ChunkXZ& coord) 
	: coord(coord), worldCoord(coord * CHUNK_SIZE) {

	minimumPoint = 256;
	highestPoint = 0;
	chunkDataGenerated = false;
}

ChunkArea::~ChunkArea() {
	for(auto& chunk : chunks)
		delete chunk.second;
}


void ChunkArea::prepare() {
	if(!nearbyChunksDetected) {
		World::getChunkManager().getNearbyChunkAreas(coord, nearbyChunkAreas);
		nearbyChunksDetected = true;
	}

	for(auto nearbyChunkArea : nearbyChunkAreas) {
		if(!nearbyChunkArea->chunkDataGenerated) {
			World::worldGenerator->generateChunkArea(*nearbyChunkArea);
			nearbyChunkArea->placeDecorations();
		}
	}

	if(!chunkDataGenerated) {
		World::worldGenerator->generateChunkArea(*this);
		placeDecorations();
	}
}

void ChunkArea::placeDecorations() {
	for(auto& plant : _plants)
		World::getChunkManager().placeBlock(plant.second, plant.first->getPlant());

	for(auto& tree : _trees) {
		Structure structure;
		if(instanceof<Desert>(tree.first))
			structure.generateCactus(tree.second);
		else {
			BlockID logBlock = AIR, leaveBlock = AIR;
			if(instanceof<BirchForest>(tree.first)) {
				logBlock = BlockID::BIRCH_LOG;
				leaveBlock = BlockID::BIRCH_LEAVE;
			}
			else {
				logBlock = BlockID::OAK_LOG;
				leaveBlock = BlockID::OAK_LEAVE;
			}

			structure.generateTree(tree.second, logBlock, leaveBlock);
		}
		structure.build();
	}
}

Chunk* ChunkArea::getChunk(const int& y) {
	ChunkXYZ chunkCoord = { coord.x, y, coord.z };

	if(!_chunkExists(chunkCoord)) {
		std::lock_guard<std::mutex> lock(chunkMutex);
		chunks.emplace(chunkCoord, new Chunk(chunkCoord));
	}

	return chunks[chunkCoord];
}


bool ChunkArea::_chunkExists(const ChunkXYZ& coord) {
	return (chunks.find(coord) != chunks.end());
}