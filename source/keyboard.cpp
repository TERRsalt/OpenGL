#include <print>
#include <map>

#include <GLFW/glfw3.h>

#include "random.hpp"

//info // Functions and variables for the input //

static void exitTheApp(GLFWwindow *window) {
    glfwSetWindowShouldClose(window, true);

    std::println("Closed app");
}

std::map<std::string, float> backgroundColor = {{"red", 1}, {"green", 1}, {"blue", 1}};

static void wireframeMode() {
    static bool wireframeModeOn = false;

    if (wireframeModeOn) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        std::println("Changed to full drawing mode");
        wireframeModeOn = false;
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        std::println("Changed to wireframe mode");
        wireframeModeOn = true;
    }
}

//info // Input //

static std::map<int, bool> wasPressedMap;
static bool wasKeyboardButtonPressed(GLFWwindow *window, int key) {
    bool isPressed = glfwGetKey(window, key) == GLFW_PRESS;
    bool justPressed = isPressed && !wasPressedMap[key];

    wasPressedMap[key] = isPressed;

    return justPressed;
}

void processInput(GLFWwindow *window) {
    if (wasKeyboardButtonPressed(window, GLFW_KEY_ESCAPE)) exitTheApp(window);

    else if (wasKeyboardButtonPressed(window, GLFW_KEY_R)) backgroundColor = randomRgbColor();

    else if (wasKeyboardButtonPressed(window, GLFW_KEY_W)) wireframeMode();
}