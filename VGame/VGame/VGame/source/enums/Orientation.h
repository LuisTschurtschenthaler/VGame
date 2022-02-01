#ifndef ORIENTATION_H
#define ORIENTATION_H


enum BlockFace {
	FACE_RIGHT = 0, // X+
	FACE_LEFT,		// X-
	FACE_TOP,		// Y+
	FACE_BOTTOM,	// Y-
	FACE_FRONT,		// Z+
	FACE_BACK,		// Z-

	TOTAL_BLOCK_FACES
};

enum NearbyChunks {
	CHUNK_RIGHT = 0, // X+
	CHUNK_LEFT,		 // X-
	CHUNK_TOP,		 // Y+
	CHUNK_BOTTOM,	 // Y-
	CHUNK_FRONT,	 // Z+
	CHUNK_BACK,		 // Z-

	TOTAL_NEARBY_CHUNKS
};

enum NearbyChunkAreas {
	CHUNK_AREA_RIGHT = 0,	// X+
	CHUNK_AREA_LEFT,		// X-
	CHUNK_AREA_FRONT,		// Z+
	CHUNK_AREA_BACK,		// Z-

	TOTAL_NEARBY_CHUNK_AREAS
};

#endif // ORIENTATION_H
