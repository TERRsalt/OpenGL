#include <print>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../files.hpp"
#include "../keyboard.hpp"

#include "renderer.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);}

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

    int success;
    char infoLog[512];

    //minor // Vertex shader //

    const std::string vertexShaderSource = fileRead("source/shaders/vertex.vert");
    const char *vertexShaderSourceCChar = vertexShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSourceCChar, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::println("Error! Vertex shader compilation error\n{}", infoLog);
    }

    //minor // Fragment shader //

    const std::string fragmentShaderSource = fileRead("source/shaders/fragment.frag");
    const char *fragmentShaderSourceCChar = fragmentShaderSource.c_str();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCChar, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::println("Error! Fragment shader compilation error\n{}", infoLog);
    }

    //minor // Shader program //

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::println("Error! Shader program compilation error!");
    }

    //minor // Deleting shaders //

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //info // Vertices and indices //

    float vertices[] = {
        -0.5, 0.5, 0,
        -0.5, -0.5, 0,
        0.5, -0.5, 0,
        0.5, 0.5, 0
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    //info // Vertex and buffer(s) data //

    unsigned int vertexBufferObject, vertexArrayObject, elementBufferObject;

    glGenBuffers(1, &vertexBufferObject);
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &elementBufferObject);

    //minor // Binding the Vertex Array Object and Element BufferObject //

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //exp // Unbinding the VBO and VAO //
    glBindVertexArray(0);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor["red"], backgroundColor["green"], backgroundColor["blue"], 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //minor // Drawing the object(s) //

        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);

        //glDrawArrays(GL_TRIANGLES, 0, 3); //exp // Drawing the triangle //
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //exp // Drawing the quad //

        //minor // Check for events, call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Cleanup //

    glDeleteVertexArrays(1, &vertexArrayObject);

    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);

    glDeleteProgram(shaderProgram);

    //info // Closing the window //

    glfwTerminate();

    return 0;
}