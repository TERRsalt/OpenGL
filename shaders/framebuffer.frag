#version 330 core

out vec4 FragmentColor;

in vec2 TextureCoordinates;

uniform sampler2D uScreenTexture;

void main() {
    // FragmentColor = vec4(vec3(1.0 - texture(uScreenTexture, TextureCoordinates)), 1.0); //minor // Negative //

    //minor // Grey scale //
    FragmentColor = texture(uScreenTexture, TextureCoordinates);
    float average = 0.2126 * FragmentColor.r + 0.7152 * FragmentColor.g + 0.0722 * FragmentColor.b;
    FragmentColor = vec4(average, average, average, 1.0);

    //info // Kernels //

    // const float OFFSET = 1.0 / 500.0;

    // vec2 offsets[9] = vec2[](
    //     vec2(-OFFSET, OFFSET), vec2(0.0f, OFFSET), vec2(OFFSET, OFFSET),
    //     vec2(-OFFSET, 0.0f), vec2(0.0f, 0.0f), vec2(OFFSET, 0.0f),
    //     vec2(-OFFSET, -OFFSET), vec2(0.0f, -OFFSET), vec2(OFFSET, -OFFSET)
    // );


    // float kernel[9] = float[](
    //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    //     1.0 / 16, 2.0 / 16, 1.0 / 16
    // );

    // vec3 sampleTexture[9];
    // for (int i =0; i < 9; i++) sampleTexture[i] = vec3(texture(uScreenTexture, TextureCoordinates.st + offsets[i]));

    // vec3 color = vec3(0.0);
    // for (int i = 0; i < 9; i++) color += sampleTexture[i] * kernel[i];

    // FragmentColor = vec4(color, 1.0);
}