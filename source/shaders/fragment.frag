#version 330 core

out vec4 FragmentColor;

in vec3 CustomColor;

void main() {FragmentColor = vec4(CustomColor, 1);}