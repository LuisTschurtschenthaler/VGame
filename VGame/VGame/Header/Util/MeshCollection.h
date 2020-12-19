#ifndef MESH_COLLECTION_H
#define MESH_COLLECTION_H

#include <vector>
#include "MeshTypes.h"
#include "ChunkMesh.h"

class Chunk;

struct MeshCollection {
	ChunkMesh* solid;
	ChunkMesh* fluid;
	ChunkMesh* flora;


	MeshCollection(Chunk* chunk) {
		solid = new ChunkMesh(chunk);
		fluid = new ChunkMesh(chunk);
		flora = new ChunkMesh(chunk);
	}


	void clear() {
		solid->clear();
		fluid->clear();
		flora->clear();
	}

	ChunkMesh* get(const MeshType& meshType) {
		switch(meshType) {
			case MeshType::SOLID:
				return solid;

			case MeshType::FLUID:
				return fluid;

			case MeshType::FLORA:
				return flora;
		}
	}

};

#endif // MESH_COLLECTION_H
