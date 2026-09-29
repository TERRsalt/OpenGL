#version 330 core

out vec4 FragmentColor;

in vec3 TextureCoordinates;

uniform samplerCube uSkybox;

void main() {FragmentColor = texture(uSkybox, TextureCoordinates);}