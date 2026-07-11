#include <print>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

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

    //info // Vertices (and indices), colors and texture coordinates //

    //minor // Vertices with colors and texture coordinates //

    // float vertices[] = { //minor // Vertices for triangle //
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

    float vertices[] = { //minor // Vertices for quad //
        -0.5, 0.5, 0,
        1, 0, 0,
        0, 1,

        -0.5, -0.5, 0,
        0, 1, 0,
        0, 0,

        0.5, -0.5, 0,
        1, 0, 0,
        1, 0,

        0.5, 0.5, 0,
        0, 0, 1,
        1, 1
    };

    //minor // Indices //

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
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

    unsigned int texture1, texture2;

    //minor // Loading and generating the texture //

    stbi_set_flip_vertically_on_load(true);

    int width, height, numberOfChannels;
    unsigned char* data;

    //minor // First texture //

    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("assets/coa.png", &width, &height, &numberOfChannels, 0);
    if (data) {
        int format = (numberOfChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else std::println("Failed to load the texture");

    stbi_image_free(data);

    //minor // Second texture //

    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    data = stbi_load("assets/dirt.png", &width, &height, &numberOfChannels, 0);
    if (data) {
        int format = (numberOfChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else std::println("Failed to load the texture");

    stbi_image_free(data);

    //info // Using the shader and setting up the textures //

    shader.use();

    shader.setUniform("uTexture1", 0);
    shader.setUniform("uTexture2", 1);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor["red"], backgroundColor["green"], backgroundColor["blue"], 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //minor // Drawing the object(s) //

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3); //exp // Drawing the triangle //
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //exp // Drawing the quad //

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