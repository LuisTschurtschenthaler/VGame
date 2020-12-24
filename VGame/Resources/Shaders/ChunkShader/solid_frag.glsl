#version 440 core
out vec4 color;

in vec3 vs_position;
in vec2 vs_texCoord;
in vec3 vs_normal;
in vec3 vs_viewNormal;
in float vs_ambientOcclusion;

uniform float dayTime;
uniform vec3 cameraPosition;
uniform int renderDistance;
uniform struct Light {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

uniform sampler2D textureAtlas;

const float PI = 3.14159265359;
const float dayLight = 1.f;
const float nightLight = 0.45f;


float calulateAmbient() {
    //if (dayTime >= 340 || dayTime <= 20) {
    //    float dayIntensity = sin((360 - dayTime) * PI / 2);
    //    return dayIntensity * nightLight + (1 - dayIntensity) * dayLight;
    //}
    //else if (dayTime >= 160 && dayTime <= 200) {
    //    float dayIntensity = sin((dayTime - 160) * PI / 4);
    //    return dayIntensity * nightLight + (1 - dayIntensity) * dayLight;
    //}
	if (dayTime > 200 && dayTime < 340)
        return nightLight;
    else return dayLight;
}


const float MAX_LIGHT_LEVEL = 15.f;


void main() {
	vec4 texColor = texture(textureAtlas, vs_texCoord.xy);
	if(texColor.a == 0.f) discard;
	
	float ambient = clamp((vs_ambientOcclusion +2) / 4.f, 0.5f, 1.f);
		

	//float light = clamp(15.f / MAX_LIGHT_LEVEL, 0.f, 1.f);	
	//float sun = clamp((15.f / MAX_LIGHT_LEVEL) * ((sin(dayTime * 0.01f) + 1.f) / 2.f), 0.2f, 1.f);

	vec3 result = (texColor.rgb * (ambient));
	//result = (normalize(vs_viewNormal) * 0.5f) + 0.5f;			
	
	color = (vec4(result, texColor.a));
}
