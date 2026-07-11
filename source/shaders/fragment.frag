#version 330 core

out vec4 FragmentColor;

in vec3 CustomColor;
in vec2 TextureCoordinates;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main() {FragmentColor = mix(texture(uTexture1, TextureCoordinates), texture(uTexture2, TextureCoordinates), 0.5);}