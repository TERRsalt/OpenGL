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

    Shader shader("../shaders/vertex.vert", "../shaders/fragment.frag");
    shader.use();

    //info // Vertex and buffer(s) data //

    Mesh mesh(indices);

    //info // Lighting //

    Shader lightingShader("../shaders/vertex.vert", "../shaders/lightning.frag");

    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);

    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    //exp // Lighting attributes //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    constexpr auto LIGHT_POSITION = glm::vec3(2, 2, -2);

    //info // Textures //

    backgroundColor = colors::SKY_BLUE;

    stbi_set_flip_vertically_on_load(true);

    Texture texture("assets/atlas.png", GL_NEAREST);
    shader.setUniform("uTexture", 0);

    texture.bind(0);

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

        shader.use();

        glm::mat4 view = glm::lookAt(camera.cameraPosition, camera.cameraPosition + camera.cameraFront, camera.cameraUp);
        shader.setUniform("uView", view);

        shader.setUniform("uProjection", camera.projection);

        //minor // Drawing the blocks //

        constexpr int HALF_OF_THE_CHUNK_SIZE = 4;

        mesh.updateVertices(blocks["colorGreen"]);
        for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
            for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                glm::mat4 model = glm::translate(UNIT_MATRIX, glm::vec3(x, 0, z));
                shader.setUniform("uModel", model);
                mesh.draw();
            }
        }

        mesh.updateVertices(blocks["colorBrown"]);
        for (int y = -1; y >= -2; y--) {
            for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
                for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                    glm::mat4 model = glm::translate(UNIT_MATRIX, glm::vec3(x, y, z));
                    shader.setUniform("uModel", model);
                    mesh.draw();
                }
            }
        }

        mesh.updateVertices(blocks["colorGrey"]);
        for (int y = -3; y >= -9; y--) {
            for (int x = -HALF_OF_THE_CHUNK_SIZE; x <= HALF_OF_THE_CHUNK_SIZE; x++) {
                for (int z = -HALF_OF_THE_CHUNK_SIZE; z <= HALF_OF_THE_CHUNK_SIZE; z++) {
                    glm::mat4 model = glm::translate(UNIT_MATRIX, glm::vec3(x, y, z));
                    shader.setUniform("uModel", model);
                    mesh.draw();
                }
            }
        }

        //minor // Drawing the light source //

        lightingShader.use();
        lightingShader.setUniform("uView", view);
        lightingShader.setUniform("uProjection", camera.projection);

        glm::mat4 model = glm::translate(UNIT_MATRIX, LIGHT_POSITION);
        //model = glm::scale(model, glm::vec3(0.2));
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
    texture.remove();
    debugUi::remove();

    //info // Closing the window //

    glfwTerminate();

    return 0;
}