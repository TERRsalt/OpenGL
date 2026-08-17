#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTextureCoordinates;

out vec3 FragmentPosition;
out vec3 Normal;
out vec3 CustomColor;
out vec2 TextureCoordinates;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1);
    FragmentPosition = vec3(uModel * vec4(aPosition, 1));
    Normal = aNormal;

    CustomColor = aColor;
    TextureCoordinates = aTextureCoordinates;
}