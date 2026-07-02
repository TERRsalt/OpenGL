#pragma once

#include <map>
#include <string>

int randomInt(int maxNumber);
float randomFloat(int maxNumber);

std::map<std::string, float> randomBackgroundColor(GLFWwindow *window, std::map<std::string, float> &backgroundColor);