#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>
#include <ctime>
#include <random>


class Random {

private:
	static std::mt19937 _rd;


public:
	static void init();
	static unsigned long int get(const unsigned long int& min = 0, const unsigned long int& max = 1000);
	static int isIntInRange(int min, int max);
	static bool getBool();

};

#endif // RANDOM_H
