#include <math.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "deltaTime.hpp"
#include "screen.hpp"

glm::vec3 cameraPosition = glm::vec3(0, 0, 2.5);
glm::vec3 cameraFront = glm::vec3(0, 0, -1);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

static float fov = 60;
glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

//info // Character movement //

const float speed = 5;

void movement(GLFWwindow *window) {
    float cameraSpeed = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition += cameraFront * cameraSpeed;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition -= cameraFront * cameraSpeed;
}

//info // Camera movement //

static float pitch = 0;
static float yaw = -90;

float lastMousePositionX = screenWidth / 2, lastMousePositionY = screenHeight / 2;

const float sensitivity = 0.1;

static bool isTheFirstTimeInCameraMovement = true;

void cameraMovement(GLFWwindow *window, double mousePositionX, double mousePositionY) {
    if (isTheFirstTimeInCameraMovement) { //exp // Needed to not have sudden camera jump, when starting the application //
        lastMousePositionX = mousePositionX;
        lastMousePositionY = mousePositionY;
        isTheFirstTimeInCameraMovement = false;
    }

    float offsetX = mousePositionX - lastMousePositionX;
    float offsetY = lastMousePositionY - mousePositionY; //exp // Reversed for Y, because Y coordinates are going from bottom to up //

    lastMousePositionX = mousePositionX;
    lastMousePositionY = mousePositionY;

    offsetX *= sensitivity;
    offsetY *= sensitivity;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89) pitch = 89;
    if (pitch < -89) pitch = -89;

    glm::vec3 direction = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    cameraFront = glm::normalize(direction);
}

//minor // Zooming in and out //

void zoomingInAndOut(GLFWwindow *window, double offsetX, double offsetY) {
    fov -= static_cast<float>(offsetY * 5);

    if (fov < 30) fov = 30;
    if (fov > 60) fov = 60;

    projection = glm::perspective(glm::radians(fov), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
}