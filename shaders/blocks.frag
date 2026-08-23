#version 330 core

out vec4 FragmentColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec3 CustomColor;
in vec2 TextureCoordinates;

uniform vec3 uViewPosition;
uniform sampler2D uTexture;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material uMaterial;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light uLight;

void main() {
    vec3 ambient = uLight.ambient * uMaterial.ambient;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(uLight.position - FragmentPosition);
    float floatDiffuse = max(dot(normal, lightDirection), 0);
    vec3 diffuse = uLight.diffuse * (floatDiffuse * uMaterial.diffuse);

    float specularStrength = 0.5;
    vec3 viewDirection = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float floatSpecular = pow(max(dot(viewDirection, reflectDirection), 0), uMaterial.shininess);
    vec3 specular = uLight.specular * (floatSpecular * uMaterial.specular);

    vec3 phong = ambient + diffuse + specular;

    FragmentColor = vec4(texture(uTexture, TextureCoordinates).rgb * phong, 1);
}