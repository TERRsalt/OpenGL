#pragma once

#include <map>
#include <string>

#include <GLFW/glfw3.h>

extern std::map<std::string, float> backgroundColor;

void processInput(GLFWwindow *window);