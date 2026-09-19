#include <print>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ranges>

#include "files.hpp"
#include "keyboard.hpp"
#include "camera.hpp"
#include "init.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "blocks.hpp"
#include "colors.hpp"
#include "mesh.hpp"
#include "time.hpp"
#include "debugUi.hpp"
#include "model.hpp"

int main() {
    GLFWwindow *window = init();

    //info // Shaders //

    Shader blocksWithLightingShader("shaders/vertex.vert", "shaders/blocksWithLighting.frag");

    // Shader singleColorShaderShader("shaders/vertex.vert", "shaders/singleColorShader.frag");

    Shader transparentShader("shaders/vertex.vert", "shaders/transparent.frag");

    Shader lightSourceShader("shaders/vertex.vert", "shaders/lightSource.frag");

    //info // Meshes and models //

    stbi_set_flip_vertically_on_load(true);

    const Mesh mesh(indices);

    Model backpackModel = Model("assets/backpack/backpack.obj");

    //info // Lighting //

    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);

    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    //exp // Lighting attributes //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    //info // Textures //

    backgroundColor = colors::SKY_BLUE;

    const Texture atlas2("assets/atlas2.png", GL_LINEAR);
    atlas2.bind(0);
    blocksWithLightingShader.setUniform("uMaterial.diffuse", 0);

    const Texture atlas2Specular("assets/atlas2Specular.png", GL_LINEAR);
    atlas2Specular.bind(1);
    blocksWithLightingShader.setUniform("uMaterial.specular", 1);

    //info // Camera //

    glfwSetWindowUserPointer(window, &camera);

    //info // Floating cubes //

    const std::vector<glm::vec3> floatingCubes = generateFloatingCubes();

    //info // Point lights positions //

    const std::vector<glm::vec3> POINT_LIGHTS_POSITIONS = {
        {1.0f, 1.0f, -1.0f},
        {0.0f, -5.0f, -6.0f},
        {5.0f, -10.0f, -5.0f},
        {-10.0f, 10.0f, -10.0f},
    };

    //info // Running the window //

    constexpr auto UNIT_MATRIX = glm::mat4(1);

    while (!glfwWindowShouldClose(window)) {
        //minor // Delta time //

        gameTime.updateDeltaTime();

        //minor // Processing the input //

        processingTheInput(window);

        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            camera.cameraMovement(window);
            camera.characterMovement(window);
            camera.zoomingInAndOut(window);
        }

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // glStencilFunc(GL_ALWAYS, 0, 0xFF);

        //minor // Directional light //

        blocksWithLightingShader.use();

        blocksWithLightingShader.setUniform("uDirectionalLight.direction", glm::vec3(-100.0f, -100.0f, -200.0f));

        blocksWithLightingShader.setUniform("uDirectionalLight.ambient", glm::vec3(0.05f));
        blocksWithLightingShader.setUniform("uDirectionalLight.diffuse", glm::vec3(2.0f));
        blocksWithLightingShader.setUniform("uDirectionalLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        //minor // Point light //

        blocksWithLightingShader.setUniform("uPointLights[0].position", POINT_LIGHTS_POSITIONS[0]);

        blocksWithLightingShader.setUniform("uPointLights[0].ambient", glm::vec3(0.05f));
        blocksWithLightingShader.setUniform("uPointLights[0].diffuse", glm::vec3(2.0f));
        blocksWithLightingShader.setUniform("uPointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        blocksWithLightingShader.setUniform("uPointLights[0].constant", 1.0f);
        blocksWithLightingShader.setUniform("uPointLights[0].linear", 0.09f);
        blocksWithLightingShader.setUniform("uPointLights[0].quadratic", 0.032f);

        blocksWithLightingShader.setUniform("uPointLights[1].position", POINT_LIGHTS_POSITIONS[1]);

        blocksWithLightingShader.setUniform("uPointLights[1].ambient", glm::vec3(0.05f));
        blocksWithLightingShader.setUniform("uPointLights[1].diffuse", glm::vec3(2.0f));
        blocksWithLightingShader.setUniform("uPointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        blocksWithLightingShader.setUniform("uPointLights[1].constant", 1.0f);
        blocksWithLightingShader.setUniform("uPointLights[1].linear", 0.09f);
        blocksWithLightingShader.setUniform("uPointLights[1].quadratic", 0.032f);

        blocksWithLightingShader.setUniform("uPointLights[2].position", POINT_LIGHTS_POSITIONS[2]);

        blocksWithLightingShader.setUniform("uPointLights[2].ambient", glm::vec3(0.05f));
        blocksWithLightingShader.setUniform("uPointLights[2].diffuse", glm::vec3(2.0f));
        blocksWithLightingShader.setUniform("uPointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        blocksWithLightingShader.setUniform("uPointLights[2].constant", 1.0f);
        blocksWithLightingShader.setUniform("uPointLights[2].linear", 0.09f);
        blocksWithLightingShader.setUniform("uPointLights[2].quadratic", 0.032f);

        blocksWithLightingShader.setUniform("uPointLights[3].position", POINT_LIGHTS_POSITIONS[3]);

        blocksWithLightingShader.setUniform("uPointLights[3].ambient", glm::vec3(0.0f));
        blocksWithLightingShader.setUniform("uPointLights[3].diffuse", glm::vec3(2.0f));
        blocksWithLightingShader.setUniform("uPointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        blocksWithLightingShader.setUniform("uPointLights[3].constant", 1.0f);
        blocksWithLightingShader.setUniform("uPointLights[3].linear", 0.09f);
        blocksWithLightingShader.setUniform("uPointLights[3].quadratic", 0.032f);

        //minor // Spot light //

        blocksWithLightingShader.setUniform("uSpotLight.position", camera.position);
        blocksWithLightingShader.setUniform("uSpotLight.direction", camera.front);
        if (flashlight) {
            blocksWithLightingShader.setUniform("uSpotLight.cutOff", glm::cos(glm::radians(20.0f)));
            blocksWithLightingShader.setUniform("uSpotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
        }
        else {
            blocksWithLightingShader.setUniform("uSpotLight.cutOff", glm::cos(glm::radians(0.0f)));
            blocksWithLightingShader.setUniform("uSpotLight.outerCutOff", glm::cos(glm::radians(0.0f)));
        }

        blocksWithLightingShader.setUniform("uSpotLight.ambient", glm::vec3(0.05f));
        blocksWithLightingShader.setUniform("uSpotLight.diffuse", glm::vec3(2.0f));
        blocksWithLightingShader.setUniform("uSpotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        //minor // Other stuff //

        blocksWithLightingShader.setUniform("uMaterial.shininess", 16.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas2.textureId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, atlas2Specular.textureId);

        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.cameraUp);

        glm::mat4 model;

        //minor // Drawing the blocks //

        constexpr int HALF_OF_THE_CHUNK_SIZE = 4;

        blocksWithLightingShader.setViewAndDirection(view);

        mesh.updateVertices(blocks["container"]);
        for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
            for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                model = glm::translate(UNIT_MATRIX, glm::vec3(x, 0, z));
                blocksWithLightingShader.setUniform("uModel", model);
                mesh.draw();
            }
        }

        // mesh.updateVertices(blocks["container"]);
        for (int y = -1; y >= -2; y--) {
            for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
                for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                    model = glm::translate(UNIT_MATRIX, glm::vec3(x, y, z));
                    blocksWithLightingShader.setUniform("uModel", model);
                    mesh.draw();
                }
            }
        }

        // mesh.updateVertices(blocks["container"]);
        for (int y = -3; y >= -9; y--) {
            for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
                for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                    model = glm::translate(UNIT_MATRIX, glm::vec3(x, y, z));
                    blocksWithLightingShader.setUniform("uModel", model);
                    mesh.draw();
                }
            }
        }

        //minor // Drawing the floating cubes //

        auto degrees = static_cast<float>(glfwGetTime() * 30);
        float additionalDegrees = 0;
        for (glm::vec3 floatingCube: floatingCubes) {
            model = glm::translate(UNIT_MATRIX, floatingCube);
            model = glm::rotate(model, glm::radians(degrees + additionalDegrees), glm::vec3(1.0f, 0.3f, 0.5f));
            additionalDegrees += 30;
            blocksWithLightingShader.setUniform("uModel", model);
            mesh.draw();
        }

        //minor // Stencil //

        // glStencilFunc(GL_ALWAYS, 1, 0xFF);
        // glStencilMask(0xFF);
        // mesh.updateVertices(blocks["container"]);
        // model = glm::translate(UNIT_MATRIX, glm::vec3(-3, 1, -3));
        // shader.setUniform("uModel", model);
        // mesh.draw();
        //
        // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // glStencilMask(0x00);
        // glDisable(GL_DEPTH_TEST); //exp // Needed for a true X-ray //
        // singleColorShader.use();
        // singleColor.setViewAndDirection(view);
        //
        // mesh.updateVertices(blocks["container"]);
        // model = glm::translate(UNIT_MATRIX, glm::vec3(-3, 1, -3));
        // model = glm::scale(model, glm::vec3(1.1));
        // singleColorShader.setUniform("uModel", model);
        // mesh.draw();
        //
        // glStencilFunc(GL_ALWAYS, 1, 0xFF);
        // glStencilMask(0xFF);
        // glEnable(GL_DEPTH_TEST);

        //minor // Drawing the backpack model //

        // shader.use();
        // model = glm::translate(UNIT_MATRIX, glm::vec3(0.0f, -20.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(1.0f));
        // shader.setUniform("uModel", model);
        // backpackModel.draw(shader);

        //minor // Drawing the light source //

        lightSourceShader.use();
        lightSourceShader.setViewAndDirection(view);

        lightSourceShader.setUniform("uColor", glm::vec3(1.0f));

        for (auto lightPosition: POINT_LIGHTS_POSITIONS) {
            model = glm::translate(UNIT_MATRIX, lightPosition);
            lightSourceShader.setUniform("uModel", model);
            mesh.draw();
        }

        //minor // Drawing the grass and glass (transparent "blocks") //

        glEnable(GL_CULL_FACE);

        transparentShader.use();
        transparentShader.setViewAndDirection(view);

        std::vector<TransparentBlockPosition> transparentBlocksPosition = {
            {"grass", {3.0f, 1.05f, 3.0f}},
            {"glass", {-3.0f, 1.05f, -3.0f}},
            {"glass", {0.0f, 1.05f, 3.0f}}
        };

        std::multimap<float, TransparentBlockPosition> sorted;
        for (const auto &transparentBlockPosition: transparentBlocksPosition) {
            float distance = glm::length(camera.position - transparentBlockPosition.position);
            sorted.emplace(distance, transparentBlockPosition);
        }

        for (auto &transparentBlock: std::views::reverse(sorted)) {
            const TransparentBlockPosition &block = transparentBlock.second;
            mesh.updateVertices(blocks[block.type]);
            model = glm::translate(UNIT_MATRIX, block.position);
            transparentShader.setUniform("uModel", model);

            glCullFace(GL_FRONT);
            mesh.draw();
            glCullFace(GL_BACK);
            mesh.draw();
        }

        glDisable(GL_CULL_FACE);

        //minor // ImGui //

        if (fpsAndTheCoordinatesDebugMenu) debugUi::fpsAndTheCoordinates();

        if (debugMenu) debugUi::debug();

        //minor // Check for events, call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Closing the window //

    glfwTerminate();

    return 0;
}
