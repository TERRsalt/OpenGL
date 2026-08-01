#include <print>
#include <map>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "random.hpp"
#include "camera.hpp"

//info // Functions and variables for the input //

static void pause(GLFWwindow *window) {
    static bool isPaused = false;
    isPaused = !isPaused;

    if (isPaused == false) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        camera.isTheFirstTimeInCameraMovement = true;
    }
    else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

glm::vec3 backgroundColor = {1, 1, 1};
static glm::vec3 randomRgbColor() {
    glm::vec3 randomColor = {randomFloat(1), randomFloat(1), randomFloat(1)};

    std::println("Changed the color to RGB({}, {}, {})", randomColor.r, randomColor.g, randomColor.b);

    return randomColor;
}

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

bool debugMenu = false;

//info // Input //

static std::map<int, bool> wasPressedMap;
static bool wasKeyboardButtonPressed(GLFWwindow *window, int key) {
    bool isPressed = glfwGetKey(window, key) == GLFW_PRESS;
    bool justPressed = isPressed && !wasPressedMap[key];

    wasPressedMap[key] = isPressed;

    return justPressed;
}

void processingTheInput(GLFWwindow *window) {
    if (wasKeyboardButtonPressed(window, GLFW_KEY_ESCAPE)) pause(window);

    else if (wasKeyboardButtonPressed(window, GLFW_KEY_R)) backgroundColor = randomRgbColor();

    else if (wasKeyboardButtonPressed(window, GLFW_KEY_Q)) wireframeMode();

    else if (wasKeyboardButtonPressed(window, GLFW_KEY_F3)) debugMenu = !debugMenu;
}