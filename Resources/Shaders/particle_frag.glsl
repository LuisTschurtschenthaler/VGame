#version 440 core
out vec4 color;

in vec2 vs_texCoord;

uniform sampler2D textureAtlas;


void main() {
	vec4 textureColor = texture(textureAtlas, vs_texCoord.xy);
	if(textureColor.a == 0.f) discard;

	vec3 result = textureColor.rgb;
	color = (vec4(result, textureColor.a));
}
