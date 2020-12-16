#ifndef ARRAY_2D_H
#define ARRAY_2D_H

#include <array>

template<class T, int SIZE>
class Array2D {

private:
	std::array<std::array<T, SIZE>, SIZE> _data;


public:
	void fill(const T& value) {
		for(int x = 0; x < SIZE; x++)
			_data[x].fill(value);
	}

	const T& get(int x, int y) const {
		return _data[x][y];
	}

	void set(int x, int y, const T& value) {
		_data[x][y] = value;
	}

};

#endif // ARRAY_2D_H
