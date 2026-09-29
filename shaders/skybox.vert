#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 TextureCoordinates;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    TextureCoordinates = aPosition;
    gl_Position = uProjection * uView * vec4(aPosition, 1.0);
}