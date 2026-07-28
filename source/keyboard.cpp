#include <print>
#include <map>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "random.hpp"

//info // Functions and variables for the input //

static void exitTheApp(GLFWwindow *window) {
    glfwSetWindowShouldClose(window, true);

    std::println("Closed app");
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

std::vector<glm::vec3> cubes = {
    { 0, 0, 0},
    { 2, 5, -15},
    {-1.5, -2.2, -2.5},
    {-3.8, -2, -12.3},
    { 2.4, -0.4, -3.5},
    {-1.7, 3, -7.5},
    { 1.3, -2, -2.5},
    { 1.5, 2, -2.5},
    { 1.5, 0.2, -1.5},
    {-1.3, 1, -1.5}
};
static void randomCubes() {
    float randomCoordinates[3] = {randomFloat(10) - 5, randomFloat(10) - 5, -randomFloat(10)};
    cubes.emplace_back(randomCoordinates[0], randomCoordinates[1], randomCoordinates[2]);

    std::println("Placed a cube in [ {} {} {} ] location", randomCoordinates[0], randomCoordinates[1], randomCoordinates[2]);
}

//info // Input //

static std::map<int, bool> wasPressedMap;
static bool wasKeyboardButtonPressed(GLFWwindow *window, int key) {
    bool isPressed = glfwGetKey(window, key) == GLFW_PRESS;
    bool justPressed = isPressed && !wasPressedMap[key];

    wasPressedMap[key] = isPressed;

    return justPressed;
}

void processingTheInput(GLFWwindow *window) {
    if (wasKeyboardButtonPressed(window, GLFW_KEY_ESCAPE)) exitTheApp(window);

    else if (wasKeyboardButtonPressed(window, GLFW_KEY_R)) backgroundColor = randomRgbColor();

    else if (wasKeyboardButtonPressed(window, GLFW_KEY_Q)) wireframeMode();

    //else if (wasKeyboardButtonPressed(window, GLFW_KEY_C)) randomCubes();
}