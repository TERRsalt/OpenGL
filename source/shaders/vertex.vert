#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;

out vec3 CustomColor;
out vec2 TextureCoordinates;

uniform mat4 transformationMatrix;

void main() {
    gl_Position = transformationMatrix*vec4(aPosition, 1);
    CustomColor = aColor;
    TextureCoordinates = aTextureCoordinates;
}