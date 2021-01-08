#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include "Coordinates.h"
class Chunk;

class FileManager {

public:
	static const std::string CHUNK_SAVE_PATH;


public:
	static void save(Chunk& chunk);
	static void load(const ChunkCoordXYZ& coord);

};


#endif // FILE_MANAGER_H
