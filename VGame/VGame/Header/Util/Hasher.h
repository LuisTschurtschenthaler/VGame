#ifndef HASHER_H
#define HASHER_H

template<class T>
inline void hash_combine(size_t seed, const T& v) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

#endif // HASHER_H
