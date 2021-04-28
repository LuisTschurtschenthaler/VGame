#version 440 core
out vec4 color;

in vec3 vs_position;


uniform float dayTime;

const vec3 dayColorTop		= vec3(11, 90, 182) / 255.f;
const vec3 dayColorBottom	= vec3(57, 182, 214) / 255.f;
const vec3 nightColorTop	= vec3(17, 85, 155) / 255.f;
const vec3 nightColorBottom = vec3(19, 21, 22) / 255.f;


void main() {
	float point = (vs_position.y + 1.f) / 2.f;
	float actualDayTime = (sin(dayTime * 0.01f) + 1.f) / 2.f;
	
	vec3 dayColor = mix(dayColorBottom, dayColorTop, point),
		 nightColor = mix(nightColorBottom, nightColorTop, point);

	vec3 finalColor = mix(nightColor, dayColor, actualDayTime);
	color = vec4(finalColor, 1.f);
}
