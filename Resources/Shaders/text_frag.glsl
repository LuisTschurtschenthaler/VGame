#version 440 core
out vec4 color;

in vec2 vs_texCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main() {    
    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(text, vs_texCoords).r);
    color = vec4(textColor, 1.f) * sampled;
}