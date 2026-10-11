#version 330 core

out vec4 FragmentColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 uCameraPosition;
uniform samplerCube uSkybox;

void main() {
    vec3 cameraDirection = normalize(Position - uCameraPosition);
    vec3 reflection = reflect(cameraDirection, normalize(Normal));
    FragmentColor = vec4(texture(uSkybox, reflection).rgb, 1.0);

    // float reflectionRatio = 1.0 / 1.52;
    // vec3 cameraDirection = normalize(Position - uCameraPosition);
    // vec3 reflection = refract(cameraDirection, normalize(Normal), reflectionRatio);
    // FragmentColor = vec4(texture(uSkybox, reflection).rgb, 1.0);
}