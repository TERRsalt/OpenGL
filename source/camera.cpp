#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "deltaTime.hpp"

glm::vec3 cameraPosition = glm::vec3(0, 0, 2.5);
glm::vec3 cameraFront = glm::vec3(0, 0, -1);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

const float speed = 5;

void moving(GLFWwindow *window) {
    float cameraSpeed = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition += cameraFront * cameraSpeed;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition -= cameraFront * cameraSpeed;
}