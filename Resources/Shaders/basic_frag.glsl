#version 330 core

out vec4 color;

in vec2 vs_texCoords;

uniform sampler2D test;


void main() {
    color = texture(test, vs_texCoords);
}
