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
	CHUNK_FRONT,	 // Z+
	CHUNK_BACK,		 // Z-

	TOTAL_NEARBY_CHUNKS
};

#endif // ORIENTATION_H
