#include <print>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    Shader shader("shaders/vertex.vert", "shaders/blocks.frag");
    shader.use();

    //info // Vertex and buffer(s) data //

    stbi_set_flip_vertically_on_load(true);

    const Mesh mesh(indices);

    Model backpackModel = Model("assets/backpack/backpack.obj");

    //info // Lighting //

    const Shader lightingShader("shaders/vertex.vert", "shaders/lightSource.frag");

    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);

    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    //exp // Lighting attributes //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    //info // Textures //

    backgroundColor = colors::SKY_BLUE;

    // Texture texture("assets/atlas.png", GL_NEAREST);
    // shader.setUniform("uTexture", 0);
    // texture.bind(0);

    const Texture container("assets/container.png", GL_LINEAR);
    container.bind(0);
    shader.setUniform("uMaterial.diffuse", 0);

    const Texture containerSpecular("assets/containerSpecular.png", GL_LINEAR);
    containerSpecular.bind(1);
    shader.setUniform("uMaterial.specular", 1);

    const Texture matrixEmission("assets/matrix.jpg", GL_LINEAR);
    matrixEmission.bind(2);
    shader.setUniform("uMaterial.emission", 2);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //minor // Using the shaders and updating the view position //

        shader.use();

        shader.setUniform("uViewPosition", camera.position);

        //minor // Directional light //

        shader.setUniform("uDirectionalLight.direction", glm::vec3(-100.0f, -100.0f, -200.0f));

        shader.setUniform("uDirectionalLight.ambient", glm::vec3(0.05f));
        shader.setUniform("uDirectionalLight.diffuse", glm::vec3(2.0f));
        shader.setUniform("uDirectionalLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        //minor // Point light //

        shader.setUniform("uPointLights[0].position", POINT_LIGHTS_POSITIONS[0]);

        shader.setUniform("uPointLights[0].ambient", glm::vec3(0.05f));
        shader.setUniform("uPointLights[0].diffuse", glm::vec3(2.0f));
        shader.setUniform("uPointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.setUniform("uPointLights[0].constant", 1.0f);
        shader.setUniform("uPointLights[0].linear", 0.09f);
        shader.setUniform("uPointLights[0].quadratic", 0.032f);

        shader.setUniform("uPointLights[1].position", POINT_LIGHTS_POSITIONS[1]);

        shader.setUniform("uPointLights[1].ambient", glm::vec3(0.05f));
        shader.setUniform("uPointLights[1].diffuse", glm::vec3(2.0f));
        shader.setUniform("uPointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.setUniform("uPointLights[1].constant", 1.0f);
        shader.setUniform("uPointLights[1].linear", 0.09f);
        shader.setUniform("uPointLights[1].quadratic", 0.032f);

        shader.setUniform("uPointLights[2].position", POINT_LIGHTS_POSITIONS[2]);

        shader.setUniform("uPointLights[2].ambient", glm::vec3(0.05f));
        shader.setUniform("uPointLights[2].diffuse", glm::vec3(2.0f));
        shader.setUniform("uPointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.setUniform("uPointLights[2].constant", 1.0f);
        shader.setUniform("uPointLights[2].linear", 0.09f);
        shader.setUniform("uPointLights[2].quadratic", 0.032f);

        shader.setUniform("uPointLights[3].position", POINT_LIGHTS_POSITIONS[3]);

        shader.setUniform("uPointLights[3].ambient", glm::vec3(0.0f));
        shader.setUniform("uPointLights[3].diffuse", glm::vec3(2.0f));
        shader.setUniform("uPointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.setUniform("uPointLights[3].constant", 1.0f);
        shader.setUniform("uPointLights[3].linear", 0.09f);
        shader.setUniform("uPointLights[3].quadratic", 0.032f);

        //minor // Spot light //

        shader.setUniform("uSpotLight.position", camera.position);
        shader.setUniform("uSpotLight.direction", camera.front);
        shader.setUniform("uSpotLight.cutOff", glm::cos(glm::radians(20.0f)));
        shader.setUniform("uSpotLight.outerCutOff", glm::cos(glm::radians(25.0f)));

        shader.setUniform("uSpotLight.ambient", glm::vec3(0.05f));
        shader.setUniform("uSpotLight.diffuse", glm::vec3(2.0f));
        shader.setUniform("uSpotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        //minor // Other stuff //

        shader.setUniform("uMaterial.shininess", 16.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.textureId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerSpecular.textureId);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, matrixEmission.textureId);

        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.cameraUp);
        shader.setUniform("uView", view);

        shader.setUniform("uProjection", camera.projection);

        //minor // Drawing the blocks //

        constexpr int HALF_OF_THE_CHUNK_SIZE = 4;
        glm::mat4 model;

        mesh.updateVertices(blocks["container"]);
        for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
            for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                model = glm::translate(UNIT_MATRIX, glm::vec3(x, 0, z));
                shader.setUniform("uModel", model);
                mesh.draw();
            }
        }

        mesh.updateVertices(blocks["container"]);
        for (int y = -1; y >= -2; y--) {
            for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
                for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                    model = glm::translate(UNIT_MATRIX, glm::vec3(x, y, z));
                    shader.setUniform("uModel", model);
                    mesh.draw();
                }
            }
        }

        mesh.updateVertices(blocks["container"]);
        for (int y = -3; y >= -9; y--) {
            for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
                for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                    model = glm::translate(UNIT_MATRIX, glm::vec3(x, y, z));
                    shader.setUniform("uModel", model);
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
            shader.setUniform("uModel", model);
            mesh.draw();
        }

        //minor // Drawing the backpack model //

        shader.use();

        model = glm::translate(UNIT_MATRIX, glm::vec3(0.0f, -20.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        shader.setUniform("uModel", model);
        backpackModel.draw(shader);

        //minor // Drawing the light source //

        lightingShader.use();

        lightingShader.setUniform("uView", view);
        lightingShader.setUniform("uProjection", camera.projection);

        lightingShader.setUniform("uColor", glm::vec3(1.0f));

        for (auto lightPosition: POINT_LIGHTS_POSITIONS) {
            model = glm::translate(UNIT_MATRIX, lightPosition);
            lightingShader.setUniform("uModel", model);
            mesh.draw();
        }

        //minor // ImGui //

        if (debugMenu) debugUi::debug();

        //minor // Check for events, call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Cleanup //

    shader.remove();
    mesh.remove();
    // texture.remove();
    container.remove();
    containerSpecular.remove();
    matrixEmission.remove();
    debugUi::remove();

    //info // Closing the window //

    glfwTerminate();

    return 0;
}
