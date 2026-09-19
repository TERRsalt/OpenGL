#version 330 core

out vec4 FragmentColor;

in vec2 TextureCoordinates;

uniform sampler2D uTexture;

void main() {
    vec4 textureColor = texture(uTexture, TextureCoordinates);
    if (textureColor.a < 0.1) discard;
    FragmentColor = textureColor;
}