#ifndef UTIL_H
#define UTIL_H

#include <string>

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}


class Util {

public:
	static std::string angleToTime(float angle) {
		std::string time = "";

		float decimalValue = 3.f - (1.f / 30.f) * (static_cast<int>(angle) % 360);
		if(decimalValue < 0)
			decimalValue += 12.f;

		int hours = static_cast<int>(decimalValue);
		if(hours == 0) hours = 12;
	
		time += (hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours)) + ":";
	
		int minutes = (int) (decimalValue * 60) % 60;
		time += minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
		return time;
	}
};


#endif // UTIL_H
