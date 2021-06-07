#ifndef CHUNK_AREA_H
#define CHUNK_AREA_H

#include <unordered_map>
#include <mutex>
#include <vector>
#include "Coordinates.h"
#include "Chunk.h"
#include "Biome.h"

class WorldGenerator;


class ChunkArea {

	friend class WorldGenerator;

public:
	std::mutex chunkMutex;
	const ChunkXZ coord, worldCoord;
	std::unordered_map<ChunkXYZ, Chunk*> chunks;
	ChunkArea* nearbyChunkAreas[TOTAL_NEARBY_CHUNK_AREAS];
	
	int highestPoint, minimumPoint;
	bool chunkDataGenerated, nearbyChunksDetected;

private:
	std::vector<std::pair<Biome*, LocationXYZ>> _plants, _trees;


public:
	ChunkArea(const ChunkXZ& coord);
	~ChunkArea();

	void prepare();
	void placeDecorations();
	Chunk* getChunk(const int& y);

private:
	bool _chunkExists(const ChunkXYZ& coord);

};

#endif // CHUNK_AREA_H
