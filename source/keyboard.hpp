#pragma once

#include <map>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern std::map<std::string, float> backgroundColor;

void processInput(GLFWwindow *window);

extern std::vector<glm::vec3> cubes;