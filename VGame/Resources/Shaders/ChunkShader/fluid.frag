#version 440 core
out vec4 color;

in vec3 vs_position;
in vec2 vs_texCoord;
in vec3 vs_normal;

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
uniform bool isPlayerUnderwater;


const float PI = 3.14159265359;
const float dayLight = 1.f;
const float nightLight = 0.65f;


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


void main() {
	vec4 texColor = texture(textureAtlas, vs_texCoord.xy);
	if(texColor.a == 0.f) discard;

	// Ambient
	float ambient = calulateAmbient();
	
	// Diffuse
	vec3 lightDir = normalize(light.direction);
	float diff = max(dot(vs_normal, lightDir), 0.f);
	vec3 diffuse = light.diffuse * diff;
	
	// Specular
	//vec3 viewDir = normalize(cameraPosition - vs_position);
	//vec3 reflectDir = normalize(reflect(lightDir, vs_normal));
	//float spec = pow(max(dot(viewDir, reflectDir), 0.f), 20.f);
	//vec3 specular = light.specular * spec;
	
	// Attenuation
	float dist = length(lightDir - vs_position);
	float attenuation = 1.f / (1.f + 0.09f * dist + 0.0032f * (dist * dist));
	ambient *= attenuation;
	diffuse *= attenuation;
	//specular *= attenuation;

	vec3 result = vec3(ambient);
	//if((dayTime >= 0 && dayTime <= 195) || dayTime >= 340)
	//	result += vec3(diffuse);

	// Ambient occlusion
	//int rd = renderDistance*16;
	//float z = min(gl_FragCoord.z / gl_FragCoord.w + 1.f, rd);
	//vec3 normal = normalize(texColor.xyz * 2.f - 1.f);
	//
	//vec3 rs = (ambient) * texColor.xyz;
	//vec3 c = rs * (clamp(dot(normal, lightDir), 0.05f, 0.5f)) * 1.f;
	
	color = (vec4(result, 1.f) * texColor);
}
