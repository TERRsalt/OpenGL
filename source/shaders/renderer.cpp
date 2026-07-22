#include <print>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../files.hpp"
#include "../keyboard.hpp"

#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertices.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int renderer() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //info // Window //

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //info // GLEW (instead of GLAD) //

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::println("Failed to initialize GLEW");
        return -1;
    }

    //info // Depth testing //

    glEnable(GL_DEPTH_TEST);

    //info // Shaders //

    Shader shader("source/shaders/vertex.vert", "source/shaders/fragment.frag");
    shader.use();

    //info // Vertex and buffer(s) data //

    unsigned int vbo, vao;
    unsigned int ebo;

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);

    //minor // Binding the Vertex Array Object and Element BufferObject //

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(sizeof(float)*vertices.size()), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(sizeof(unsigned int)*indices.size()), indices.data(), GL_STATIC_DRAW);

    //exp // Position attributes //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    //exp // Color attributes //
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //exp // Texture attributes //
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), reinterpret_cast<void*>(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //exp // Unbinding the VBO and VAO //
    glBindVertexArray(0);

    //info // Textures //

    stbi_set_flip_vertically_on_load(true);

    unsigned int dirtTexture = texture("assets/dirt.png", GL_NEAREST);
    shader.setUniform("uTexture", 0);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor["red"], backgroundColor["green"], backgroundColor["blue"], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //minor // Drawing the object(s) //

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dirtTexture);

        //shader.use();

        //minor // Transforming the matrices and sending it to vertex.vert ^-^ //

        static const glm::mat4 unitMatrix = glm::mat4(1);

        glm::mat4 view = glm::translate(unitMatrix, glm::vec3(0, 0, -3));
        shader.setUniform("uView", view);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);
        shader.setUniform("uProjection", projection);

        //minor // Drawing the triangle/quad/cube(s) //

        glBindVertexArray(vao);

        //glDrawArrays(GL_TRIANGLES, 0, 3); //exp // Drawing the triangle //
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //exp // Drawing the quad //

        //exp // Drawing the cube(s) //
        for (int i = 0; i < cubes.size(); i++) {
            glm::mat4 model = glm::translate(unitMatrix, cubes[i]);
            model = glm::rotate(model, glm::radians(20.0f*static_cast<float>(i)+static_cast<float>(glfwGetTime()*25)), glm::vec3(1, 0.3, 0.5));

            shader.setUniform("uModel", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        //minor // Check for events, call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Cleanup //

    shader.remove();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteTextures(1, &dirtTexture);

    //info // Closing the window //

    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);}