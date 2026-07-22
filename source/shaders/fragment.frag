#version 330 core

out vec4 FragmentColor;

in vec3 CustomColor;
in vec2 TextureCoordinates;

uniform sampler2D uTexture;

void main() {FragmentColor = texture(uTexture, TextureCoordinates);}