#include "Util.h"


const float Util::getDistance(glm::vec2 d1, glm::vec2 d2) {
	return glm::length(d1 - d2);
}

const glm::mat4& Util::getModel(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	model = glm::scale(model, scale);
	return model;
}

std::string Util::yawToCardinalPoint(const float& yaw) {
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


bool Util::_isInRange(const float& yaw, const int& numberToCheckPos, const int& numberToCheckNeg) {
	float minPos = numberToCheckPos - CARDIAL_POINT_RANGE,
		  maxPos = numberToCheckPos + CARDIAL_POINT_RANGE;
	float minNeg = numberToCheckNeg - CARDIAL_POINT_RANGE,
		  maxNeg = numberToCheckNeg + CARDIAL_POINT_RANGE;

	return ((yaw >= minPos) && (yaw <= maxPos) || (yaw >= minNeg) && (yaw <= maxNeg));
}
