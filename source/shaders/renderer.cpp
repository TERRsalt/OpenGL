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

    //info // Vertices (and indices), colors and texture coordinates //

    //minor // Vertices with colors and texture coordinates //

    // float vertices[] = { //minor // Vertices for the triangle //
    //     0, 0.5, 0,
    //     1, 0, 0,
    //     0.5, 1,
    //
    //     -0.5, -0.5, 0,
    //     0, 1, 0,
    //     0, 0,
    //
    //     0.5, -0.5, 0,
    //     0, 0, 1,
    //     1, 0
    // };

    // float vertices[] = { //minor // Vertices for the quad //
    //     -0.5, 0.5, 0,
    //     1, 0, 0,
    //     0, 1,
    //
    //     -0.5, -0.5, 0,
    //     0, 1, 0,
    //     0, 0,
    //
    //     0.5, -0.5, 0,
    //     1, 0, 0,
    //     1, 0,
    //
    //     0.5, 0.5, 0,
    //     0, 0, 1,
    //     1, 1
    // };
    // unsigned int indices[] = { //minor // Indices for the quad //
    //     0, 1, 2,
    //     0, 2, 3
    // };

    float vertices[] = { //minor // Vertices for the cube //
        //minor // Front face //

        -0.5, -0.5, 0.5,
        1, 0, 0,
        0, 0,

        0.5, -0.5, 0.5,
        0, 1, 0,
        1, 0,

        0.5, 0.5, 0.5,
        0, 0, 1,
        1, 1,

        -0.5, 0.5, 0.5,
        1, 1, 0,
        0, 1,

        //minor // Back face //

        0.5, -0.5, -0.5,
        1, 0, 0,
        0, 0,

        -0.5, -0.5, -0.5,
        0, 1, 0,
        1, 0,

        -0.5, 0.5, -0.5,
        0, 0, 1,
        1, 1,

        0.5, 0.5, -0.5,
        1, 1, 0,
        0, 1,

        //minor // Left face //

        -0.5, -0.5, -0.5,
        1, 0, 0,
        0, 0,

        -0.5, -0.5, 0.5,
        0, 1, 0,
        1, 0,

        -0.5, 0.5, 0.5,
        0, 0, 1,
        1, 1,

        -0.5, 0.5, -0.5,
        1, 1, 0,
        0, 1,

        //minor // Right face //

        0.5, -0.5, 0.5,
        1, 0, 0,
        0, 0,

        0.5, -0.5, -0.5,
        0, 1, 0,
        1, 0,

        0.5, 0.5, -0.5,
        0, 0, 1,
        1, 1,

        0.5, 0.5, 0.5,
        1, 1, 0,
        0, 1,

        //minor // Top face //

        -0.5, 0.5, 0.5,
        1, 0, 0,
        0, 0,

        0.5, 0.5, 0.5,
        0, 1, 0,
        1, 0,
        
        0.5, 0.5, -0.5,
        0, 0, 1,
        1, 1,

        -0.5, 0.5, -0.5,
        1, 1, 0,
        0, 1,

        //minor // Bottom face //
        
        -0.5, -0.5, -0.5,
        1, 0, 0,
        0, 0,

        0.5, -0.5, -0.5,
        0, 1, 0,
        1, 0,

        0.5, -0.5, 0.5,
        0, 0, 1,
        1, 1,

        -0.5, -0.5, 0.5,
        1, 1, 0,
        0, 1
    };

    unsigned int indices[] = {
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

    //info // Vertex and buffer(s) data //

    unsigned int vbo, vao;
    unsigned int ebo;

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);

    //minor // Binding the Vertex Array Object and Element BufferObject //

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

    unsigned int texture1 = texture("assets/coa.png", GL_LINEAR);
    shader.setUniform("uTexture1", 0);
    unsigned int texture2 = texture("assets/dirt.png", GL_NEAREST);
    shader.setUniform("uTexture2", 1);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor["red"], backgroundColor["green"], backgroundColor["blue"], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //minor // Drawing the object(s) //

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        //minor // Transforming the matrixes and sending it to vertex.vert ^-^ //

        static const glm::mat4 unitMatrix = glm::mat4(1);

        glm::mat4 model = unitMatrix;
        //model = glm::translate(model, glm::vec3(-0.5, 0.5, 0));
        //model = glm::rotate(model, -static_cast<float>(glfwGetTime()*0.5), glm::vec3(1, 0.75, 0.5));
        model = glm::rotate(model, static_cast<float>(glfwGetTime()*0.75), glm::vec3(1, 0.5, 0.25));
        //model = glm::scale(model, glm::vec3(0.75, 0.75, 1));
        shader.setUniform("uModel", model);

        glm::mat4 view = glm::translate(unitMatrix, glm::vec3(0, 0, -3));
        shader.setUniform("uView", view);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);
        shader.setUniform("uProjection", projection);

        //minor // Drawing the triangle/quad //

        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3); //exp // Drawing the triangle //
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //exp // Drawing the quad //
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); //exp // Drawing the cube //

        //minor // Check for events, call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Cleanup //

    shader.remove();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    //info // Closing the window //

    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);}