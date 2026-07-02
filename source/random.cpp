#include <random>
#include <print>

#include <GLFW/glfw3.h>

#include "random.hpp"

int randomInt(int maxNumber) {return rand()%(maxNumber+1);}
float randomFloat(int maxNumber) {return (float)randomInt(maxNumber*100)/100;}

std::map<std::string, float> randomBackgroundColor(GLFWwindow *window, std::map<std::string, float> &backgroundColor) {
    bool isPressed = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    static bool wasPressed = false;

    if (isPressed && !wasPressed) {
        backgroundColor["red"] = randomFloat(1);
        backgroundColor["green"] = randomFloat(1);
        backgroundColor["blue"] = randomFloat(1);

        std::println("Changed the color to RGB({}, {}, {})", backgroundColor["red"], backgroundColor["green"], backgroundColor["blue"]);
    }

    wasPressed = isPressed;

    return backgroundColor;
}
