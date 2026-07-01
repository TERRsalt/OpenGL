#include <print>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void framebufferSizeCallback(GLFWwindow *window, int screenWidth, int screenHeight) {glViewport(0, 0, screenWidth, screenHeight);}

void processInput(GLFWwindow *window) {if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);}

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
        std::println("Failed to initalize GLEW");
        return -1;
    }

    //info // Viewport //

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //info // Running the window //

    while (!glfwWindowShouldClose(window)) {
        //minor // Input //

        processInput(window);

        //minor // Rendering //

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        //minor // Check and call events and swap the buffers //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //info // Closing the window //

    glfwTerminate();

    return 0;
}