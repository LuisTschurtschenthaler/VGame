#ifndef ARRAY_3D_H
#define ARRAY_3D_H

#include <array>
#include <algorithm>
#include "Coordinates.h"


template<typename T, size_t WIDTH, size_t HEIGHT, size_t DEPTH>
class Array3D {

private:
	std::array<std::array<std::array<T, DEPTH>, HEIGHT>, WIDTH> _data;


public:
	void fill(const T& value) {
		for(size_t x = 0; x < WIDTH; x++)
		for(size_t y = 0; y < HEIGHT; y++)
			_data[x][y].fill(value);
	}

	const T& get(const size_t& x, const size_t& y, const size_t& z) const {
		return _data[x][y][z];
	}

	const T& get(const LocationXYZ& loc) const {
		return _data[loc.x][loc.y][loc.z];
	}

	void set(const size_t& x, const size_t& y, const size_t& z, const T& value) {
		_data[x][y][z] = value;
	}

	void set(const LocationXYZ& loc, const T& value) {
		_data[loc.x][loc.y][loc.z] = value;
	}
};

#endif // ARRAY_3D_H
