#ifndef ARRAY_3D_H
#define ARRAY_3D_H

#include <array>
#include <vector>
#include <algorithm>
#include "Coordinates.h"


template<class T, int WIDTH, int HEIGHT, int DEPTH>
class Array3D {

private:
	std::array<std::array<std::array<T, DEPTH>, HEIGHT>, WIDTH> _data;


public:
	void fill(const T& value) {
		std::fill(
			&_data[0][0][0],
			&_data[0][0][0] + sizeof(_data) / sizeof(_data[0][0][0]),
			value);
	}

	const T& get(int x, int y, int z) const {
		return _data[x][y][z];
	}

	const T& get(const LocationXYZ& loc) const {
		return _data[loc.x][loc.y][loc.z];
	}

	void set(int x, int y, int z, const T& value) {
		_data[x][y][z] = value;
	}

	void set(const LocationXYZ& loc, const T& value) {
		_data[loc.x][loc.y][loc.z] = value;
	}

};

#endif // ARRAY_3D_H
