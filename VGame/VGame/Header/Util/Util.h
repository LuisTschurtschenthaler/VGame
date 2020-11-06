#ifndef UTIL_H
#define UTIL_H


template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

#endif // UTIL_H
