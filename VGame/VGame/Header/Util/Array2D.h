#ifndef ARRAY_2D_H
#define ARRAY_2D_H

#include <array>

template<class T, int SIZE>
class Array2D {

private:
	std::array<T, SIZE * SIZE> _data;


public:
	void fill(const T& value) {
		_data.fill(value);
	}

	T& get(int x, int y) {
		if(_isOutOfRange(x) || _isOutOfRange(y))
			return _data[0];

		return _data[x + y * SIZE];
	}

	void set(int x, int y, const T& value) {
		if(_isOutOfRange(x) || _isOutOfRange(y))
			return;

		_data[x + y * SIZE] = value;
	}

private:
	bool _isOutOfRange(int i) {
		return (i >= SIZE) || (i < 0);
	}

};

#endif // ARRAY_2D_H
