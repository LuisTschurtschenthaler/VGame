#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>
#include <ostream>
#include <string>
#include "Hasher.h"


struct ChunkXZ {
	int x, z;

	ChunkXZ() : x(0), z(0) { }
	ChunkXZ(int x, int z) : x(x), z(z) { }

	bool operator==(const ChunkXZ& chunk) const {
		return (x == chunk.x && z == chunk.z);
	}

	bool operator<(const ChunkXZ& chunk) const {
		return (x < chunk.x || z < chunk.z);
	}

	ChunkXZ operator*(int val) const {
		return { x * val, z * val };
	}

	ChunkXZ operator+(const ChunkXZ& coord) const {
		return { x + coord.x, z + coord.z };
	}

	friend std::ostream& operator<<(std::ostream& output, const ChunkXZ& chunk) {
		return output << "ChunkXZ(" << chunk.x << ", " << chunk.z << ")";
	}
};

struct ChunkXYZ {
	int x, y, z;

	ChunkXYZ()	: x(0), y(0), z(0) { }
	ChunkXYZ(int x, int y, int z) : x(x), y(y), z(z) { }

	bool operator==(const ChunkXYZ& chunk) const {
		return (x == chunk.x && y == chunk.y && z == chunk.z);
	}

	bool operator<(const ChunkXYZ& chunk) const {
		return (x < chunk.x || y < chunk.y || z < chunk.z);
	}

	friend std::ostream& operator<<(std::ostream& output, const ChunkXYZ& chunk) {
		return output << "ChunkXYZ(" << chunk.x << ", " << chunk.y << ", " << chunk.z << ")";
	}
};

struct LocationXYZ {
	int x, y, z;

	LocationXYZ(): x(0), y(0), z(0) { }

	LocationXYZ(int x, int y, int z)
		: x(x), y(y), z(z) { }


	LocationXYZ operator+(const LocationXYZ& coord) const {
		return { x + coord.x, y + coord.y, z + coord.z };
	}

	LocationXYZ operator-(const LocationXYZ& coord) const {
		return { x - coord.x, y - coord.y, z - coord.z };
	}

	bool operator==(const LocationXYZ& coord) {
		return ((x == coord.x) && (y == coord.y) && (z == coord.z));
	}

	friend std::ostream& operator<<(std::ostream& output, const LocationXYZ& coord) {
		return output << "LocationXYZ(" << coord.x << ", " << coord.y << ", " << coord.z << ")";
	}
};


namespace std {
	template<> struct hash<ChunkXZ> {
		std::size_t operator()(const ChunkXZ& v) const {
			std::size_t seed = 0;
			hash_combine(seed, v.x);
			hash_combine(seed, v.z);

			return seed;
		}
	};

	template<> struct hash<ChunkXYZ> {
		std::size_t operator()(const ChunkXYZ& v) const {
			std::size_t seed = 0;
			hash_combine(seed, v.x);
			hash_combine(seed, v.y);
			hash_combine(seed, v.z);

			return seed;
		}
	};

	template<> struct hash<LocationXYZ> {
		std::size_t operator()(const LocationXYZ& v) const {
			std::size_t seed = 0;
			hash_combine(seed, v.x);
			hash_combine(seed, v.y);
			hash_combine(seed, v.z);

			return seed;
		}
	};
}

#endif // COORDINATES_H
