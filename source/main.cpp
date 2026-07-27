#include <print>
#include <cmath>

#include <GL/glew.h>
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
#include "vertices.hpp"
#include "mesh.hpp"
#include "time.hpp"

int main() {
    GLFWwindow *window = init();

    //info // Shaders //

    Shader shader("../shaders/vertex.vert", "../shaders/fragment.frag");
    shader.use();

    //info // Vertex and buffer(s) data //

    Mesh mesh(vertices, indices);

    //info // Textures //

    stbi_set_flip_vertically_on_load(true);

    Texture texture("assets/dirt.png", GL_NEAREST);
    shader.setUniform("uTexture", 0);

    texture.bind(0);

    //info // Camera //

    Camera camera;

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, Camera::cameraMovement);
    glfwSetScrollCallback(window, Camera::zoomingInAndOut);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        //minor // Delta time //

        gameTime.updateDeltaTime();

        //minor // Processing the input //

        processingTheInput(window);
        camera.characterMovement(window);

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //minor // Drawing the object(s) //

        //minor // Transforming the matrices and sending it to vertex.vert ^-^ //

        constexpr auto unitMatrix = glm::mat4(1);

        glm::mat4 view = glm::lookAt(camera.cameraPosition, camera.cameraPosition + camera.cameraFront, camera.cameraUp);
        shader.setUniform("uView", view);

        shader.setUniform("uProjection", camera.projection);

        //minor // Drawing the triangle/quad/cube(s) //

        //exp // Drawing the cube(s) //
        for (int i = 0; i < cubes.size(); i++) {
            glm::mat4 model = glm::translate(unitMatrix, cubes[i]);
            model = glm::rotate(model, glm::radians(20.0f * static_cast<float>(i) + static_cast<float>(glfwGetTime() * 25)), glm::vec3(1, 0.3, 0.5));

            shader.setUniform("uModel", model);

            mesh.draw();
        }

        //minor // Check for events, call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Cleanup //

    shader.remove();
    mesh.remove();
    texture.remove();

    //info // Closing the window //

    glfwTerminate();

    return 0;
}