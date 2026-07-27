#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern glm::vec3 backgroundColor;

void processingTheInput(GLFWwindow *window);

extern std::vector<glm::vec3> cubes;