#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include "colors.hpp"

std::vector<float> colorBlockGenerator(glm::vec3 colorOfTheBlock) {
    return {
        //minor // Front face //

        -0.5, -0.5, 0.5,
        0, 0, 1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, -0.5, 0.5,
        0, 0, 1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, 0.5, 0.5,
        0, 0, 1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, 0.5, 0.5,
        0, 0, 1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        //minor // Back face //

        0.5, -0.5, -0.5,
        0, 0, -1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, -0.5, -0.5,
        0, 0, -1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, 0.5, -0.5,
        0, 0, -1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, 0.5, -0.5,
        0, 0, -1,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        //minor // Left face //

        -0.5, -0.5, -0.5,
        -1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, -0.5, 0.5,
        -1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, 0.5, 0.5,
        -1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, 0.5, -0.5,
        -1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        //minor // Right face //

        0.5, -0.5, 0.5,
        1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, -0.5, -0.5,
        1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, 0.5, -0.5,
        1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, 0.5, 0.5,
        1, 0, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        //minor // Top face //

        -0.5, 0.5, 0.5,
        0, 1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, 0.5, 0.5,
        0, 1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, 0.5, -0.5,
        0, 1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, 0.5, -0.5,
        0, 1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        //minor // Bottom face //

        -0.5, -0.5, -0.5,
        0, -1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, -0.5, -0.5,
        0, -1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        0.5, -0.5, 0.5,
        0, -1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        0, 0,

        -0.5, -0.5, 0.5,
        0, -1, 0,
        colorOfTheBlock.r, colorOfTheBlock.g, colorOfTheBlock.b,
        1, 1
    };
}

//info // Generating block from texture atlas //

glm::vec4 getTextureFromAtlas(glm::vec2 textureCoordinates) {
    constexpr unsigned int ATLAS_SIZE = 16;

    float flippedY = ATLAS_SIZE - 1 - textureCoordinates.y;
    constexpr float BLEED_MARGIN = 0.005f; //exp // It's here to not see "bleeding" (lines between blocks) //

    return {
        (textureCoordinates.x + BLEED_MARGIN) / ATLAS_SIZE,
        (textureCoordinates.x + 1 - BLEED_MARGIN) / ATLAS_SIZE,
        (flippedY + BLEED_MARGIN) / ATLAS_SIZE,
        (flippedY + 1 - BLEED_MARGIN) / ATLAS_SIZE
    };
}

std::vector<float> blockGenerator(glm::ivec2 frontTextureCoordinates, glm::ivec2 backTextureCoordinates,
        glm::ivec2 leftTextureCoordinates, glm::ivec2 rightTextureCoordinates, glm::ivec2 topTextureCoordinates, glm::ivec2 bottomTextureCoordinates) {
    glm::vec4 frontTexture = getTextureFromAtlas(frontTextureCoordinates), backTexture = getTextureFromAtlas(backTextureCoordinates);
    glm::vec4 leftTexture = getTextureFromAtlas(leftTextureCoordinates), rightTexture = getTextureFromAtlas(rightTextureCoordinates);
    glm::vec4 topTexture = getTextureFromAtlas(topTextureCoordinates), bottomTexture = getTextureFromAtlas(bottomTextureCoordinates);

    return {
        //minor // Front face //

        -0.5, -0.5, 0.5,
        0, 0, 1,
        1, 0, 0,
        frontTexture[0], frontTexture[2],

        0.5, -0.5, 0.5,
        0, 0, 1,
        0, 1, 0,
        frontTexture[1], frontTexture[2],

        0.5, 0.5, 0.5,
        0, 0, 1,
        0, 0, 1,
        frontTexture[1], frontTexture[3],

        -0.5, 0.5, 0.5,
        0, 0, 1,
        1, 1, 0,
        frontTexture[0], frontTexture[3],

        //minor // Back face //

        0.5, -0.5, -0.5,
        0, 0, -1,
        1, 0, 0,
        backTexture[0], backTexture[2],

        -0.5, -0.5, -0.5,
        0, 0, -1,
        0, 1, 0,
        backTexture[1], backTexture[2],

        -0.5, 0.5, -0.5,
        0, 0, -1,
        0, 0, 1,
        backTexture[1], backTexture[3],

        0.5, 0.5, -0.5,
        0, 0, -1,
        1, 1, 0,
        backTexture[0], backTexture[3],

        //minor // Left face //

        -0.5, -0.5, -0.5,
        -1, 0, 0,
        1, 0, 0,
        leftTexture[0], leftTexture[2],

        -0.5, -0.5, 0.5,
        -1, 0, 0,
        0, 1, 0,
        leftTexture[1], leftTexture[2],

        -0.5, 0.5, 0.5,
        -1, 0, 0,
        0, 0, 1,
        leftTexture[1], leftTexture[3],

        -0.5, 0.5, -0.5,
        -1, 0, 0,
        1, 1, 0,
        leftTexture[0], leftTexture[3],

        //minor // Right face //

        0.5, -0.5, 0.5,
        1, 0, 0,
        1, 0, 0,
        rightTexture[0], rightTexture[2],

        0.5, -0.5, -0.5,
        1, 0, 0,
        0, 1, 0,
        rightTexture[1], rightTexture[2],

        0.5, 0.5, -0.5,
        1, 0, 0,
        0, 0, 1,
        rightTexture[1], rightTexture[3],

        0.5, 0.5, 0.5,
        1, 0, 0,
        1, 1, 0,
        rightTexture[0], rightTexture[3],

        //minor // Top face //

        -0.5, 0.5, 0.5,
        0, 1, 0,
        1, 0, 0,
        topTexture[0], topTexture[2],

        0.5, 0.5, 0.5,
        0, 1, 0,
        0, 1, 0,
        topTexture[1], topTexture[2],

        0.5, 0.5, -0.5,
        0, 1, 0,
        0, 0, 1,
        topTexture[1], topTexture[3],

        -0.5, 0.5, -0.5,
        0, 1, 0,
        1, 1, 0,
        topTexture[0], topTexture[3],

        //minor // Bottom face //

        -0.5, -0.5, -0.5,
        0, -1, 0,
        1, 0, 0,
        bottomTexture[0], bottomTexture[2],

        0.5, -0.5, -0.5,
        0, -1, 0,
        0, 1, 0,
        bottomTexture[1], bottomTexture[2],

        0.5, -0.5, 0.5,
        0, -1, 0,
        0, 0, 1,
        bottomTexture[1], bottomTexture[3],

        -0.5, -0.5, 0.5,
        0, -1, 0,
        1, 1, 0,
        bottomTexture[0], bottomTexture[3]
    };
}

//info // Indices //

std::vector<unsigned int> indices = {
    //minor // Front face //
    0, 1, 2,
    0, 2, 3,

    //minor // Back face //
    4, 5, 6,
    4, 6, 7,

    //minor // Left face //
    8, 9, 10,
    8, 10, 11,

    //minor // Right face //
    12, 13, 14,
    12, 14, 15,

    //minor // Top face //
    16, 17, 18,
    16, 18, 19,

    //minor // Bottom face //
    20, 21, 22,
    20, 22, 23
};

//info // Blocks //

std::map<std::string, std::vector<float>> blocks = {
    {"colorGrey", colorBlockGenerator(colors::GREY)},
    {"colorBrown", colorBlockGenerator(colors::BROWN)},
    {"colorGreen", colorBlockGenerator(colors::GREEN)},

    {"stone", blockGenerator({1, 0}, {1, 0},
        {1, 0}, {1, 0}, {1, 0}, {1, 0})},

    {"dirt", blockGenerator({2, 0}, {2, 0},
        {2, 0}, {2, 0}, {2, 0}, {2, 0})},

    {"grass", blockGenerator({3, 0}, {3, 0},
        {3, 0}, {3, 0}, {8, 2}, {2, 0})},

    {"wood", blockGenerator({4, 1}, {4, 1},
        {4, 1}, {4, 1}, {5, 1}, {5, 1})}
};