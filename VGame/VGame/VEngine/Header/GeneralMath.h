#ifndef MATH_H
#define MATH_H

#include "Constants.h"

namespace Math {

	float smoothstep(float edge0, float edge1, float x) {
		x = x * x * (3 - 2 * x);
		return (edge0 * x) + (edge1 * (1.f - x));
	}


	float smoothInterpolation(float bottomLeft, float topLeft, float bottomRight, float topRight, 
							  float xMin, float xMax, float zMin, float zMax, float x, float z) {
		
		float width = xMax - xMin, height = zMax - zMin;
		float xValue = 1 - (x - xMin) / width;
		float zValue = 1 - (z - zMin) / height;

		float a = smoothstep(bottomLeft, bottomRight, xValue);
		float b = smoothstep(topLeft, topRight, xValue);
		return smoothstep(a, b, zValue);
	}

	float bilinearInterpolation(float bottomLeft, float topLeft, float bottomRight,
								float topRight, float xMin, float xMax, float zMin,
								float zMax, float x, float z) noexcept {

		float width, height,
			xDistanceToMax, yDistanceToMax,
			xDistanceToMin, yDistanceToMin;

		width = xMax - xMin;
		height = zMax - zMin;
		xDistanceToMax = xMax - x;
		yDistanceToMax = zMax - z;
		xDistanceToMin = x - xMin;
		yDistanceToMin = z - zMin;

		return 1.f / (width * height) *
			(
			bottomLeft  * xDistanceToMax * yDistanceToMax +
			bottomRight * xDistanceToMin * yDistanceToMax +
			topLeft * xDistanceToMax * yDistanceToMin +
			topRight * xDistanceToMin * yDistanceToMin
			);
	}
}

#endif // MATH_H
