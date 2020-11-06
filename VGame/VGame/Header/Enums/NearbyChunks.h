#ifndef NEARBY_CHUNKS_H
#define NEARBY_CHUNKS_H

enum NearbyChunks {
	CHUNK_RIGHT		= 0, // X+
	CHUNK_LEFT		= 1, // X-
	//CHUNK_TOP		= 2, // Y+
	//CHUNK_BOTTOM	= 3, // Y-
	CHUNK_FRONT		= 2, // Z+
	CHUNK_BACK		= 3, // Z-

	AMOUNT_OF_NEARBY_CHUNKS
};

#endif // NEARBY_CHUNKS_H
