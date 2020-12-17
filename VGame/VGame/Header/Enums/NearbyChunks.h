#ifndef NEARBY_CHUNKS_H
#define NEARBY_CHUNKS_H

enum NearbyChunks {
	CHUNK_RIGHT		= 0, // X+
	CHUNK_LEFT		= 1, // X-
	CHUNK_FRONT		= 2, // Z+
	CHUNK_BACK		= 3, // Z-

	CHUNK_RF = 4,
	CHUNK_RB = 5,
	CHUNK_LF = 6,
	CHUNK_LB = 7,

	AMOUNT_OF_NEARBY_CHUNKS
};

#endif // NEARBY_CHUNKS_H
