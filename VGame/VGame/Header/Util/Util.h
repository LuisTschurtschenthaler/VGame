#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "Constants.h"

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}


class Util {

public:
	static std::string angleToTime(float angle) {
		std::string time = "";
		return time;
	}

	static std::string yawToCardinalPoint(float yaw) {
		if(_isInRange(yaw, 0, 0) || _isInRange(yaw, 360, -360))
			return "N";

		else if(_isInRange(yaw, 315, -45))
			return "NW";

		else if(_isInRange(yaw, 270, -90))
			return "W";

		else if(_isInRange(yaw, 225, -135))
			return "SW";

		else if(_isInRange(yaw, 180, -180))
			return "S";

		else if(_isInRange(yaw, 135, -225))
			return "SE";

		else if(_isInRange(yaw, 90, -270))
			return "E";

		else if(_isInRange(yaw, 45, -315))
			return "NE";

		else return "N/A";
	}



private:
	static bool _isInRange(float yaw, int numberToCheckPos, int numberToCheckNeg) {
		int minPos = numberToCheckPos - CARDIAL_POINT_RANGE,
			maxPos = numberToCheckPos + CARDIAL_POINT_RANGE;
		int minNeg = numberToCheckNeg - CARDIAL_POINT_RANGE,
			maxNeg = numberToCheckNeg + CARDIAL_POINT_RANGE;

		return ((yaw >= minPos) && (yaw <= maxPos) || (yaw >= minNeg) && (yaw <= maxNeg));
	}
};

#endif // UTIL_H
