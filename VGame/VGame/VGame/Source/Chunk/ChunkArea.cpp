#include "ChunkArea.h"
#include "World.h"
#include "ChunkManager.h"
#include "WorldGenerator.h"
#include "Structure.h"
#include "Util.h"


ChunkArea::ChunkArea(const ChunkXZ& coord) 
	: coord(coord) {

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
			nearbyChunkArea->setDecorations();
		}
	}

	if(!chunkDataGenerated) {
		World::worldGenerator->generateChunkArea(*this);
		setDecorations();
	}
}

void ChunkArea::setDecorations() {
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
	ChunkXYZ c = { coord.x, y, coord.z };
	if(!_chunkExists(c)) {
		chunks.emplace(c, new Chunk(c));
	}

	return chunks[c];
}


bool ChunkArea::_chunkExists(const ChunkXYZ& coord) {
	return (chunks.find(coord) != chunks.end());
}