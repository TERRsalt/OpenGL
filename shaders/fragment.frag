#version 330 core

out vec4 FragmentColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec3 CustomColor;
in vec2 TextureCoordinates;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform sampler2D uTexture;

void main() {
    float ambientLightning = 0.33;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(uLightPosition - FragmentPosition);

    float floatDiffuse = max(dot(normal, lightDirection), 0);
    vec3 diffuse = floatDiffuse * uLightColor;

    FragmentColor = vec4(texture(uTexture, TextureCoordinates).rgb * (ambientLightning + diffuse), 1);
}