#ifndef ARRAY_2D_H
#define ARRAY_2D_H

#include <array>
#include <vector>
#include <algorithm>


template<class T, size_t WIDTH, size_t HEIGHT>
class Array2D {

private:
	std::array<std::array<T, HEIGHT>, WIDTH> _data;


public:
	void fill(const T& value) {
		for(size_t x = 0; x < WIDTH; x++)
			_data[x].fill(value);
	}

	const T& get(const size_t& x, const size_t& y) const {
		return _data[x][y];
	}

	void set(const size_t& x, const size_t& y, const T& value) {
		_data[x][y] = value;
	}

};

#endif // ARRAY_2D_H
