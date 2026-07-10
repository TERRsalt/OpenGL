#include <print>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../files.hpp"
#include "../keyboard.hpp"

#include "renderer.hpp"
#include "shader.hpp"

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

    //info // Shaders //

    Shader shader("source/shaders/vertex.vert", "source/shaders/fragment.frag");

    //info // Vertices and indices //

    float vertices[] = { //minor // Vertices for triangle //
        0, 0.5, 0,
        1, 0, 0,

        -0.5, -0.5, 0,
        0, 1, 0,

        0.5, -0.5, 0,
        0, 0, 1
    };

    // float vertices[] = { //minor // Vertices for quad //
    //     -0.5, 0.5, 0,
    //     1, 0, 0,
    //
    //     -0.5, -0.5, 0,
    //     0, 1, 0,
    //
    //     0.5, -0.5, 0,
    //     1, 0, 0,
    //
    //     0.5, 0.5, 0,
    //     0, 0, 1
    // };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    //info // Vertex and buffer(s) data //

    unsigned int vbo, vao, ebo;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    //exp // Color attributes //
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //exp // Unbinding the VBO and VAO //
    glBindVertexArray(0);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor["red"], backgroundColor["green"], backgroundColor["blue"], 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //minor // Drawing the object(s) //

        shader.use();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3); //exp // Drawing the triangle //
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //exp // Drawing the quad //

        //minor // Check for events, call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Cleanup //

    glDeleteVertexArrays(1, &vao);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    //todo // Check later how to delete this shader //

    //info // Closing the window //

    glfwTerminate();

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);}