#version 440 core

in vec3 vs_position;
in vec3 vs_sunPosition;

out vec4 color;

//uniform samplerCube skyBox;
uniform sampler2D skyTextureDay;
uniform sampler2D textureSun;
uniform sampler2D skyTextureNight;
uniform sampler2D textureMoon;
uniform float weather;
uniform float time;


void main() {
	//vec3 pos = normalize(vs_position);
	//float dist = dot(vs_sunPosition, vs_position);
	//
    //vec3 colorDay = texture(skyTextureDay, vec2((vs_sunPosition.y + 1.f) / 2.f, max(0.01f, vs_position.y))).rgb;
	//vec3 colorNight = texture(skyTextureNight, vec2((vs_sunPosition.y + 1.f) / 2.f, max(0.01f, vs_position.y))).rgb;
	//
	//vec3 finalColor;
	//if(time > 0)
	//	finalColor = weather * colorNight;
	//else
	//	finalColor = weather * colorDay;

	vec3 finalColor;
	float radius = length(vs_position - vs_sunPosition);
	if(radius < 0.05f) {
		float time = clamp(vs_sunPosition.y, 0.01f, 1.f);
		radius /= 0.05f;
	
		if(radius < 1.f - 0.001f) {
			vec4 sunColor = texture(textureSun, vec2(radius, time));
			finalColor = sunColor.rgb;
		}
	} else {
		finalColor = vec3(0.2f, 0.3f, 0.7f);
	}

    color = vec4(finalColor, 1.f);
}