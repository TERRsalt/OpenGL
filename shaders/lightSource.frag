#version 330 core

out vec4 FragmentColor;

uniform vec3 uColor;

void main() {FragmentColor = vec4(uColor, 1);}