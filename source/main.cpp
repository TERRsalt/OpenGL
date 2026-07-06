#include <print>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "files.hpp"
#include "keyboard.hpp"

#include "shaders/renderer.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() {
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

    //info // GLEW (instead of GLAD) //

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::println("Failed to initialize GLEW");
        return -1;
    }

    //info // Viewport //

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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

    //info // Vertex data //

    unsigned int vertexBufferObjects;
    glGenBuffers(1, &vertexBufferObjects);

    //minor // Shader //

    const std::string vertexShaderSource = fileRead("source/shaders/vertex.vert");
    const char *vertexShaderSourceCChar = vertexShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSourceCChar, nullptr);
    glCompileShader(vertexShader);

    //minor // Errors //

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::println("Error! Vertex shader compilation error\n{}", infoLog);
    }

    //info // Fragment shader //

    //minor // Shader //

    const std::string fragmentShaderSource = fileRead("source/shaders/fragment.frag");
    const char *fragmentShaderSourceCChar = fragmentShaderSource.c_str();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCChar, nullptr);
    glCompileShader(fragmentShader);

    //minor // Errors //

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::println("Error! Fragment shader compilation error\n{}", infoLog);
    }

    //info // Shader program //

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //minor // Errors //

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::println("Error! Shader program compilation error!");
    }

    //minor // Program activation //

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //info // Vertex Array Object //

    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);

    //minor // Binding Vertex Array Object //

    glBindVertexArray(vertexArrayObject);

    //minor // Copying vertices into OpenGL buffer //

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //minor // Set vertex attributes pointers //

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    //info // Element Buffer Objects //

    unsigned int elementBufferObjects;
    glGenBuffers(1, &elementBufferObjects);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjects);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        //minor // Input //

        processInput(window);

        //minor // Clearing the screen (and painting it with some color) //

        glClearColor(backgroundColor["red"], backgroundColor["green"], backgroundColor["blue"], 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //minor // Drawing the object(s) //

        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);

        //glDrawArrays(GL_TRIANGLES, 0, 3); //exp // Drawing triangle //
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //exp // Drawing quad //

        //minor // Check and call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Cleanup //

    glDeleteVertexArrays(1, &vertexArrayObject);

    glDeleteBuffers(1, &vertexBufferObjects);
    glDeleteBuffers(1, &elementBufferObjects);

    glDeleteProgram(shaderProgram);

    //info // Closing the window //

    glfwTerminate();

    return 0;
}