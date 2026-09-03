#version 330 core

out vec4 FragmentColor;

in vec3 FragmentPosition;
in vec3 Normal;
in vec3 CustomColor;
in vec2 TextureCoordinates;

uniform vec3 uViewPosition;
uniform sampler2D uTexture;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
    vec3 ambient = uLight.ambient * vec3(texture(uMaterial.diffuse, TextureCoordinates));

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(uLight.position - FragmentPosition);
    float floatDiffuse = max(dot(normal, lightDirection), 0);
    vec3 diffuse = uLight.diffuse * floatDiffuse * vec3(texture(uMaterial.diffuse, TextureCoordinates));

    float specularStrength = 0.5;
    vec3 viewDirection = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float floatSpecular = pow(max(dot(viewDirection, reflectDirection), 0), uMaterial.shininess);
    vec3 specular = uLight.specular * floatSpecular * vec3(texture(uMaterial.specular, TextureCoordinates));

    vec3 phong = ambient + diffuse + specular;

    FragmentColor = vec4(texture(uTexture, TextureCoordinates).rgb * phong, 1);
}