#version 330 core

out vec4 FragmentColor;

in vec2 TextureCoordinates;

uniform sampler2D uScreenTexture;

void main() {FragmentColor = vec4(vec3(1.0 - texture(uScreenTexture, TextureCoordinates)), 1.0);}