#pragma once

#include <cmath>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "screen.hpp"
#include "time.hpp"

static constexpr float FOV = 75;

class Camera {
public:
    glm::vec3 cameraPosition = glm::vec3(0, 2, 0), cameraFront = glm::vec3(0, 0, -1), cameraUp = glm::vec3(0, 1, 0);
    float fov;
    glm::mat4 projection;

    float pitch = 0, yaw = -90, sensitivity, lastMousePositionX = 0, lastMousePositionY = 0;
    bool isTheFirstTimeInCameraMovement = true;

    float characterSpeed;

    Camera(float fov = FOV, float sensitivity = 0.1, float renderDistance = 100, float characterSpeed = 5):
        fov(fov), projection(glm::perspective(glm::radians(fov), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, renderDistance)),
        sensitivity(sensitivity), characterSpeed(characterSpeed) {}

    //info // Camera movement //

    static void cameraMovement(GLFWwindow *window, double mousePositionX, double mousePositionY) {
        auto *camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

        auto mousePositionFloatX = static_cast<float>(mousePositionX), mousePositionFloatY = static_cast<float>(mousePositionY);

        if (camera->isTheFirstTimeInCameraMovement) { //exp // Needed to not have sudden camera jump, when starting the application //
            camera->lastMousePositionX = mousePositionFloatX;
            camera->lastMousePositionY = mousePositionFloatY;
            camera->isTheFirstTimeInCameraMovement = false;
        }

        float offsetX = mousePositionFloatX - camera->lastMousePositionX;
        float offsetY = camera->lastMousePositionY - mousePositionFloatY; //exp // Reversed for Y, because Y coordinates are going from bottom to up //

        camera->lastMousePositionX = mousePositionFloatX;
        camera->lastMousePositionY = mousePositionFloatY;

        offsetX *= camera->sensitivity;
        offsetY *= camera->sensitivity;

        camera->yaw += offsetX;
        camera->pitch += offsetY;

        if (camera->pitch > 89) camera->pitch = 89;
        if (camera->pitch < -89) camera->pitch = -89;

        glm::vec3 direction = glm::vec3(cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch)), sin(glm::radians(camera->pitch)),
            sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch)));
        camera->cameraFront = glm::normalize(direction);
    }

    //minor // Zooming in and out //

    void zoomingInAndOut(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) setFovToCertainAmount(30, false);
        else setFovToCertainAmount(FOV, true);

        projection = glm::perspective(glm::radians(fov), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);
    }

    //info // Character movement //

    void characterMovement(GLFWwindow *window) {
        float characterSpeedDeltaTime = characterSpeed * gameTime.deltaTime;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * characterSpeedDeltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * characterSpeedDeltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPosition.x += cameraFront.x * characterSpeedDeltaTime;
            cameraPosition.z += cameraFront.z * characterSpeedDeltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPosition.x -= cameraFront.x * characterSpeedDeltaTime;
            cameraPosition.z -= cameraFront.z * characterSpeedDeltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraPosition.y += characterSpeedDeltaTime;
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cameraPosition.y -= characterSpeedDeltaTime;
    }

private:
    void setFovToCertainAmount(float targetFov, bool plusOrMinus) {
        if (plusOrMinus) {
            fov += 250 * gameTime.deltaTime;
            if (targetFov <= fov) fov = targetFov;
        }

        else {
            fov -= 250 * gameTime.deltaTime;
            if (targetFov >= fov) fov = targetFov;
        }
    }
};

extern Camera camera;