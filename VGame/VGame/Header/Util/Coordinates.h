#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>
#include <ostream>
#include <string>
#include "Hasher.h"
#include "WorldConstants.h"


struct ChunkCoordXZ {
	int x, z;

	ChunkCoordXZ() : x(0), z(0) { }

	ChunkCoordXZ(int x, int z)
		: x(x), z(z) { }

	bool operator==(const ChunkCoordXZ& c) const {
		return (x == c.x && z == c.z);
	}

	bool operator<(const ChunkCoordXZ& c) const {
		return (x < c.x || z < c.z);
	}

	const std::string& toString() {
		return (std::to_string(x) + " " + std::to_string(z));
	}

	friend std::ostream& operator<<(std::ostream& output, const ChunkCoordXZ& coord) {
		return output << "ChunkCoord(" << coord.x << ", " << coord.z << ")";
	}
};

struct ChunkCoordXYZ {
	int x, y, z;

	ChunkCoordXYZ()	: x(0), y(0), z(0) { }

	ChunkCoordXYZ(int x, int y, int z) : x(x), y(y), z(z) { }

	ChunkCoordXZ toChunkCoordXZ() {
		return ChunkCoordXZ(x, z);
	}

	bool operator==(const ChunkCoordXYZ& c) const {
		return (x == c.x && y == c.y && z == c.z);
	}

	bool operator<(const ChunkCoordXYZ& c) const {
		return (x < c.x || y < c.y || z < c.z);
	}

	const std::string toString() {
		return (std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z));
	}

	friend std::ostream& operator<<(std::ostream& output, const ChunkCoordXYZ& coord) {
		return output << "ChunkCoord(" << coord.x << ", " << coord.y << ", " << coord.z << ")";
	}
};


struct BlockPositionXZ {
	int x, z;

	BlockPositionXZ(int x, int z)
		: x(x), z(z) { }


	friend std::ostream& operator<<(std::ostream& output, const BlockPositionXZ& coord) {
		return output << "BlockPositionXZ(" << coord.x << ", " << coord.z << ")";
	}
};

struct BlockPositionXYZ {
	int x, y, z;

	BlockPositionXYZ(int x, int y, int z)
		: x(x), y(y), z(z) { }

	BlockPositionXYZ operator+(const BlockPositionXYZ& coord) {
		return { x + coord.x, y + coord.y, z + coord.z };
	}

	bool operator!=(const BlockPositionXYZ& coord) {
		return ((x != coord.x) && (y != coord.y) && (z != coord.z));
	}

	friend std::ostream& operator<<(std::ostream& output, const BlockPositionXYZ& coord) {
		return output << "BlockPositionXYZ(" << coord.x << ", " << coord.y << ", " << coord.z << ")";
	}
};


namespace std {template<> struct hash<ChunkCoordXZ> {
		inline size_t operator()(const ChunkCoordXZ& v) const {
			size_t seed = 0;
			::hash_combine(seed, v.x);
			::hash_combine(seed, v.z);

			return seed;
		}
	};

	template<> struct hash<ChunkCoordXYZ> {
		inline size_t operator()(const ChunkCoordXYZ& v) const {
			size_t seed = 0;
			::hash_combine(seed, v.x);
			::hash_combine(seed, v.y);
			::hash_combine(seed, v.z);

			return seed;
		}
	};


	template<> struct hash<BlockPositionXZ> {
		inline size_t operator()(const BlockPositionXZ& v) const {
			size_t seed = 0;
			::hash_combine(seed, v.x);
			::hash_combine(seed, v.z);

			return seed;
		}
	};

	template<> struct hash<BlockPositionXYZ> {
		inline size_t operator()(const BlockPositionXYZ& v) const {
			size_t seed = 0;
			::hash_combine(seed, v.x);
			::hash_combine(seed, v.y);
			::hash_combine(seed, v.z);

			return seed;
		}
	};
}

#endif // COORDINATES_H
