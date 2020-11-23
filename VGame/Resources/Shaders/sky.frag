#version 440 core

#define PI 3.14159265359
#define WATER_COLOR vec3(0.19140625, 0.250980392, 0.701960784)

in vec3 vs_pos;

out vec4 color;


uniform vec3 playerPosition;
uniform vec3 sunPosition;
uniform float time;
uniform bool isUnderwater;

const vec3 SKY_COLOR  = vec3(0.2f, 0.4f, 0.8f);
const vec3 DUSK_COLOR = vec3(1.f, 0.3f, 0.23f);


float hash(float n) { return fract((1.f + sin(n)) * 415.92653); }
float noise3D(vec3 vector) {
    float xhash = hash(round(400 * vector.x) * 37.f);
    float yhash = hash(round(400 * vector.y) * 57.f);
    float zhash = hash(round(400 * vector.z) * 67.f);
    return fract(xhash + yhash + zhash);
}


void main() {
	// Sky
	vec3 sunPos = normalize(sunPosition);
	float lightness = max(0.f, sunPos.y + 0.5);

    float atmosphereDensity = pow(1.f - vs_pos.y, 2.f);
    float scatter = 1.f - clamp(lightness, 0.f, 1.f);

    vec3 scatterColor = mix(vec3(1.f), DUSK_COLOR, scatter);
    vec3 skyDusk = mix(SKY_COLOR, scatterColor, atmosphereDensity / 1.5f);

	
	// Sun
	float sun = dot(sunPos, vs_pos);
    sun = pow(max(0.f, sun), mix(50.f, 70.f, lightness));
	
    float glow = sun;
    sun = pow(sun, 100.f) * 100.f;
    sun = clamp(sun, 0.f, 1.f);
	
    glow = pow(glow, 10.f * lightness);
    glow = clamp(glow, 0.f, 1.f) * lightness;
	
	vec3 sky = lightness * skyDusk + scatterColor * (sun + glow);
	
	//float a = 0.3f;
	//vec3 col = isUnderwater ? a * sky + (1.f - a) * WATER_COLOR : sky;
	
	color = vec4(sky, 1.f);
}
