#version 330 core

layout (location = 0) in vec3 aPosition;

out vec3 TextureCoordinates;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
    TextureCoordinates = aPosition;
    vec4 position = vec4(uProjection * uView * vec4(aPosition, 1.0));
    gl_Position = position.xyww;
}