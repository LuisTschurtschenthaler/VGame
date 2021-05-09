#include "ChunkArea.h"
#include "World.h"
#include "ChunkManager.h"
#include "WorldGenerator.h"


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
		if(!nearbyChunkArea->chunkDataGenerated)
			World::worldGenerator->generateChunkArea(*nearbyChunkArea);
	}

	if(!chunkDataGenerated)
		World::worldGenerator->generateChunkArea(*this);
}

void ChunkArea::generate() {
	for(auto it : chunks) {
		
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