#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

extern glm::vec3 cameraPosition;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern glm::mat4 projection;

void movement(GLFWwindow *window);

void cameraMovement(GLFWwindow *window, double xPosition, double yPosition);

void zoomingInAndOut(GLFWwindow *window, double mousePositionX, double mousePositionY);