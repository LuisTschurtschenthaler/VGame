#ifndef ARRAY_3D_H
#define ARRAY_3D_H

#include <array>
#include <vector>

template<class T, int SIZE>
class Array3D {

private:
	std::array<std::array<std::array<T, SIZE>, SIZE>, SIZE> _data;


public:
	void fill(const T& value) {
		for(int x = 0; x < CHUNK_SIZE; x++)
		for(int y = 0; y < CHUNK_SIZE; y++)
			_data[x][y].fill(value);
	}

	const T& get(int x, int y, int z) const {
		return _data[x][y][z];
	}

	void set(int x, int y, int z, const T& value) {
		_data[x][y][z] = value;
	}

};

#endif // ARRAY_3D_H
