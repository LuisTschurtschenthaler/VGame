#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLEW/GL/glew.h>
#include "Constants.h"
#include "Coordinates.h"


template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}


class Util {

public:
	static const glm::mat4& getModel(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
		glm::mat4 model(1.f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model = glm::scale(model, scale);
		return model;
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
