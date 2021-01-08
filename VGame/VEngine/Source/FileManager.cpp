#include <fstream>
#include "FileManager.h"
#include "Chunk.h"
#include "Log.h"


const std::string FileManager::CHUNK_SAVE_PATH = "./Resources/Saves/";


void FileManager::save(Chunk& chunk) {
	std::ofstream stream;

	std::string path = "WorldName/Chunks/" + chunk.chunkCoord.toString() + ".chunk";
	stream.open(CHUNK_SAVE_PATH + path, std::ofstream::in | std::ofstream::out | std::ofstream::trunc);

	if(!stream.is_open()) {
		//Log::write(Log::ERROR, "Saving Chunk failed: " + path);
		return;
	}

	for(int x = 0; x < CHUNK_SIZE; x++) {
		for(int y = 0; y < CHUNK_SIZE; y++) {
			for(int z = 0; z < CHUNK_SIZE; z++)
				stream << chunk.chunkData[chunk.getBlockArrayPosition(x, y, z)] << " ";
			stream << "\t";
		}
		stream << "\n";
	}

	stream.close();
}

void FileManager::load(const ChunkCoordXYZ& coord) {

}
