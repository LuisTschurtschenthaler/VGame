#ifndef CHUNK_AREA_H
#define CHUNK_AREA_H

#include <unordered_map>
#include <mutex>
#include <vector>
#include "Coordinates.h"
#include "Chunk.h"


class ChunkArea {

public:
	const ChunkXZ coord;
	std::unordered_map<ChunkXYZ, Chunk*> chunks;
	ChunkArea* nearbyChunkAreas[TOTAL_NEARBY_CHUNK_AREAS];
	
	int highestPoint, minimumPoint;
	bool chunkDataGenerated, nearbyChunksDetected;


public:
	ChunkArea(const ChunkXZ& coord);
	~ChunkArea();

	void prepare();
	void generate();
	Chunk* getChunk(const int& y);

private:
	bool _chunkExists(const ChunkXYZ& coord);

};

#endif // CHUNK_AREA_H
