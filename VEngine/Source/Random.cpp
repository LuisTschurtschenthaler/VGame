#include "Random.h"


std::mt19937 Random::_rd;


void Random::init() {
	_rd.seed(std::time(0));
}

unsigned long int Random::get(const unsigned long int& min, const unsigned long int& max) {
	static bool _first = true;

	if(min > max) return 0;

	if(_first) {
		std::srand(std::time(0));
		_first = false;
	}

	return (min + rand() % ((max + 1) - min));
}

int Random::getIntInRange(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(_rd);
}

float Random::getFloatInRange(float min, float max) {
	return (((float) rand() / RAND_MAX) * (max - min) + min);
}

bool Random::getBool() {
	return (0 + (rand() % (1 - 0 + 1)) == 1);
}
