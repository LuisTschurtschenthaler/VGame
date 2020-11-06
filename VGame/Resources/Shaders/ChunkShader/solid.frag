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

const float PI = 3.14159265359;
const vec3 dayLight = vec3(0.8f);
const vec3 nightLight = vec3(0.35f);

vec3 calulateAmbient() {
    if (dayTime >= 5.5 && dayTime <= 6.5) {
        float dayIntensity = sin((6.5 - dayTime) * PI / 2);
        return dayIntensity * nightLight + (1 - dayIntensity) * dayLight;
    }
	else if (dayTime < 5.5 || dayTime > 19) {
        return nightLight;
    }
    else if (dayTime >= 17 && dayTime <= 19) {
        float dayIntensity = sin((dayTime - 17) * PI / 4);
        return dayIntensity * nightLight + (1 - dayIntensity) * dayLight;
    }
    else {
        return dayLight;
    }
}


void main() {
	vec4 texColor = texture(textureAtlas, vs_texCoord.xy);
	if(texColor.a == 0.f) discard;

	// Ambient
	vec3 ambient = calulateAmbient();
	
	// Diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(vs_normal, lightDir), 0.f);
	vec3 diffuse = light.diffuse * diff;
	
	// Specular
	//vec3 viewDir = normalize(cameraPosition - vs_position);
	//vec3 reflectDir = normalize(reflect(lightDir, vs_normal));
	//float spec = pow(max(dot(viewDir, reflectDir), 0.f), 50.f);
	//vec3 specular = light.specular * spec;

	// Ambient occlusion
	//int rd = renderDistance;
	//float z = min(gl_FragCoord.z / gl_FragCoord.w + 1.f, rd);
	//vec3 normal = normalize(texColor.xyz * 2.f - 1.f);
	//
	//vec3 rs = (ambient) * texColor.xyz;
	//vec3 c = rs * (clamp(dot(normal, lightDir), 0.05f, 0.5f)) * 1.f;
	//vec3 ao = (c - (c - rs)) / (z / rd);
	
	// Attenuation
	//float attenuation = 1.f / (1.f + 0.45f * 4 + 0.0075f * (4 * 4));
	//ambient *= attenuation;
	//diffuse *= attenuation;
	//specular *= attenuation;

	vec3 result = (ambient );
	color = vec4(result, 1.f) * texColor;
}