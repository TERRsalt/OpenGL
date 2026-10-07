#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include "random.hpp"

//info // Generating block from texture atlas //

glm::vec4 getTextureFromAtlas(glm::vec2 textureCoordinates) {
    constexpr unsigned int atlasSize = 2; //minor // Change this value, depending on the atlas (atlasSize x atlasSize) //

    const float flippedY = atlasSize - 1 - textureCoordinates.y;
    constexpr float bleedMargin = 0.005f; //exp // It's here to not see "bleeding" (lines between blocks) //

    return {
        (textureCoordinates.x + bleedMargin) / atlasSize,
        (textureCoordinates.x + 1 - bleedMargin) / atlasSize,
        (flippedY + bleedMargin) / atlasSize,
        (flippedY + 1 - bleedMargin) / atlasSize
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
        frontTexture[0], frontTexture[2],

        0.5, -0.5, 0.5,
        0, 0, 1,
        frontTexture[1], frontTexture[2],

        0.5, 0.5, 0.5,
        0, 0, 1,
        frontTexture[1], frontTexture[3],

        -0.5, 0.5, 0.5,
        0, 0, 1,
        frontTexture[0], frontTexture[3],

        //minor // Back face //

        0.5, -0.5, -0.5,
        0, 0, -1,
        backTexture[0], backTexture[2],

        -0.5, -0.5, -0.5,
        0, 0, -1,
        backTexture[1], backTexture[2],

        -0.5, 0.5, -0.5,
        0, 0, -1,
        backTexture[1], backTexture[3],

        0.5, 0.5, -0.5,
        0, 0, -1,
        backTexture[0], backTexture[3],

        //minor // Left face //

        -0.5, -0.5, -0.5,
        -1, 0, 0,
        leftTexture[0], leftTexture[2],

        -0.5, -0.5, 0.5,
        -1, 0, 0,
        leftTexture[1], leftTexture[2],

        -0.5, 0.5, 0.5,
        -1, 0, 0,
        leftTexture[1], leftTexture[3],

        -0.5, 0.5, -0.5,
        -1, 0, 0,
        leftTexture[0], leftTexture[3],

        //minor // Right face //

        0.5, -0.5, 0.5,
        1, 0, 0,
        rightTexture[0], rightTexture[2],

        0.5, -0.5, -0.5,
        1, 0, 0,
        rightTexture[1], rightTexture[2],

        0.5, 0.5, -0.5,
        1, 0, 0,
        rightTexture[1], rightTexture[3],

        0.5, 0.5, 0.5,
        1, 0, 0,
        rightTexture[0], rightTexture[3],

        //minor // Top face //

        -0.5, 0.5, 0.5,
        0, 1, 0,
        topTexture[0], topTexture[2],

        0.5, 0.5, 0.5,
        0, 1, 0,
        topTexture[1], topTexture[2],

        0.5, 0.5, -0.5,
        0, 1, 0,
        topTexture[1], topTexture[3],

        -0.5, 0.5, -0.5,
        0, 1, 0,
        topTexture[0], topTexture[3],

        //minor // Bottom face //

        -0.5, -0.5, -0.5,
        0, -1, 0,
        bottomTexture[0], bottomTexture[2],

        0.5, -0.5, -0.5,
        0, -1, 0,
        bottomTexture[1], bottomTexture[2],

        0.5, -0.5, 0.5,
        0, -1, 0,
        bottomTexture[1], bottomTexture[3],

        -0.5, -0.5, 0.5,
        0, -1, 0,
        bottomTexture[0], bottomTexture[3]
    };
}

//info // Blocks //

std::map<std::string, std::vector<float>> blocks = {
    //minor // Atlas blocks //

    // {"stone", blockGenerator({1, 0}, {1, 0},
    //         {1, 0}, {1, 0}, {1, 0}, {1, 0})},
    // {"dirt", blockGenerator({2, 0}, {2, 0},
    //         {2, 0}, {2, 0}, {2, 0}, {2, 0})},
    // {"grass", blockGenerator({3, 0}, {3, 0},
    //         {3, 0}, {3, 0}, {8, 2}, {2, 0})},
    // {"wood", blockGenerator({4, 1}, {4, 1},
    //         {4, 1}, {4, 1}, {5, 1}, {5, 1})},

    //minor // Atlas 2 blocks //

    {"container", blockGenerator({0, 0}, {0, 0},
            {0, 0}, {0, 0}, {0, 0}, {0, 0})},

    {"grass", blockGenerator({0, 1}, {1, 0},
            {1, 0}, {1, 0}, {1, 0}, {1, 0})},

    {"glass", blockGenerator({1, 1}, {1, 1},
            {1, 1}, {1, 1}, {1, 1}, {1, 1})}
};

//info // Vector containing floating cubes //

std::vector<glm::vec3> generateFloatingCubes() {
    std::vector<glm::vec3> floatingCubes;

    for (int numberOfCubes = 0; numberOfCubes < 10; numberOfCubes++) {
        floatingCubes.emplace_back(static_cast<float>(randomInt(5) - 2.5), static_cast<float>(randomInt(5) + 5),
                static_cast<float>(randomInt(5) - 2.5));
    }

    return floatingCubes;
}