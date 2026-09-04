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

int main() {
    GLFWwindow *window = init();

    //info // Shaders //

    Shader shader("../shaders/vertex.vert", "../shaders/blocks.frag");
    shader.use();

    //info // Vertex and buffer(s) data //

    Mesh mesh(indices);

    //info // Lighting //

    Shader lightingShader("../shaders/vertex.vert", "../shaders/lightSource.frag");

    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);

    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    //exp // Lighting attributes //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    //info // Textures //

    backgroundColor = colors::SKY_BLUE;

    stbi_set_flip_vertically_on_load(true);

    // Texture texture("assets/atlas.png", GL_NEAREST);
    // shader.setUniform("uTexture", 0);
    // texture.bind(0);

    Texture container("assets/container.png", GL_LINEAR);
    container.bind(0);
    shader.setUniform("uMaterial.diffuse", 0);

    Texture containerSpecular("assets/containerSpecular.png", GL_LINEAR);
    containerSpecular.bind(1);
    shader.setUniform("uMaterial.specular", 1);

    Texture matrixEmission("assets/matrix.jpg", GL_LINEAR);
    matrixEmission.bind(2);
    shader.setUniform("uMaterial.emission", 2);

    //info // Camera //

    glfwSetWindowUserPointer(window, &camera);

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

        //minor // Transforming the matrices and sending it to vertex.vert ^-^ //

        auto lightPosition = glm::vec3(5.0f, 10.0f, -20.0f);

        shader.use();

        shader.setUniform("uViewPosition", camera.position);

        shader.setUniform("uLight.position", lightPosition);
        shader.setUniform("uLight.ambient", glm::vec3(0.2f));
        shader.setUniform("uLight.diffuse", glm::vec3(2.0f));
        shader.setUniform("uLight.specular", glm::vec3(-0.25f, -0.25f, 1.0f));

        shader.setUniform("uMaterial.shininess", 64.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.textureId);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, containerSpecular.textureId);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, matrixEmission.textureId);

        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.cameraFront, camera.cameraUp);
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

        //minor // Drawing the light source //

        lightingShader.use();

        lightingShader.setUniform("uView", view);
        lightingShader.setUniform("uProjection", camera.projection);

        lightingShader.setUniform("uColor", glm::vec3(1.0f));

        model = glm::translate(UNIT_MATRIX, lightPosition);
        // model = glm::scale(model, glm::vec3(5));
        lightingShader.setUniform("uModel", model);

        glBindVertexArray(lightVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);

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