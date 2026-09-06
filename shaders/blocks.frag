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
    sampler2D emission;

    float shininess;
};
uniform Material uMaterial;

//info // Different lighting //

//minor // Directional light //

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight uDirectionalLight;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
    vec3 lightDirection = normalize(-light.direction);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TextureCoordinates));

    float floatDiffuse = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * floatDiffuse * vec3(texture(uMaterial.diffuse, TextureCoordinates));

    float specularStrength = 0.5;
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float floatSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterial.shininess);
    vec3 specularMask = vec3(texture(uMaterial.specular, TextureCoordinates));
    vec3 specular = light.specular * floatSpecular * specularMask;

    float specularValue = specularMask.r;
    vec3 emission = vec3(texture(uMaterial.emission, TextureCoordinates)) * (vec3(1.0) - step(0.05, specularValue));

    return (ambient + diffuse + specular);
}

//minor // Point light //

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NUMBER_OF_POINT_LIGHTS 4
uniform PointLight uPointLights[NUMBER_OF_POINT_LIGHTS];

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
    vec3 lightDirection = normalize(light.position - fragmentPosition);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TextureCoordinates));

    float floatDiffuse = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * floatDiffuse * vec3(texture(uMaterial.diffuse, TextureCoordinates));

    float specularStrength = 0.5;
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float floatSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterial.shininess);
    vec3 specularMask = vec3(texture(uMaterial.specular, TextureCoordinates));
    vec3 specular = light.specular * floatSpecular * specularMask;

    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

//minor // Spotlight //

struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Spotlight uSpotLight;

vec3 calculateSpotLight(Spotlight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
    vec3 lightDirection = normalize(light.position - FragmentPosition);

    vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TextureCoordinates));

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if (theta > light.outerCutOff) {
        float floatDiffuse = max(dot(normal, lightDirection), 0.0);
        vec3 diffuse = light.diffuse * floatDiffuse * vec3(texture(uMaterial.diffuse, TextureCoordinates));

        float specularStrength = 0.5;
        vec3 viewDirection = normalize(uViewPosition - FragmentPosition);
        vec3 reflectDirection = reflect(-lightDirection, normal);
        float floatSpecular = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterial.shininess);
        vec3 specularMask = vec3(texture(uMaterial.specular, TextureCoordinates));
        vec3 specular = light.specular * floatSpecular * specularMask;

        float specularValue = specularMask.r;
        vec3 emission = vec3(texture(uMaterial.emission, TextureCoordinates)) * (vec3(1.0) - step(0.05, specularValue));

        diffuse *= intensity;
        specular *= intensity;

        return (ambient + diffuse + specular);
    }

    else return vec3(0.0);
}

//info // `main` //

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(uViewPosition - FragmentPosition);

    vec3 result = calculateDirectionalLight(uDirectionalLight, normal, viewDirection);
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++) result += calculatePointLight(uPointLights[i], normal, FragmentPosition, viewDirection);
    result += calculateSpotLight(uSpotLight, normal, FragmentPosition, viewDirection);

    FragmentColor = vec4(result, 1.0);
}