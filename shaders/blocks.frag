#version 330 core

out vec4 FragmentColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec3 CustomColor;
in vec2 TextureCoordinates;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;
uniform vec3 uViewPosition;
uniform sampler2D uTexture;

void main() {
    float ambient = 0.33;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(uLightPosition - FragmentPosition);
    float floatDiffuse = max(dot(normal, lightDirection), 0);
    vec3 diffuse = floatDiffuse * uLightColor;

    float specularStrength = 0.5;
    vec3 viewDirection = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float floatSpecular = pow(max(dot(viewDirection, reflectDirection), 0), 8);
    vec3 specular = specularStrength * floatSpecular * uLightColor;

    FragmentColor = vec4(texture(uTexture, TextureCoordinates).rgb * (ambient + diffuse + specular), 1);
}