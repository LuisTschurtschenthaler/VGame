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
	static const float getDistance(glm::vec2 d1, glm::vec2 d2);
	static const glm::mat4& getModel(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	static std::string yawToCardinalPoint(const float& yaw);

private:
	static bool _isInRange(const float& yaw, const int& numberToCheckPos, const int& numberToCheckNeg);

};

#endif // UTIL_H
