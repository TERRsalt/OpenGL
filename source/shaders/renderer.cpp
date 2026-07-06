#include <GLFW/glfw3.h>

void framebufferSizeCallback(GLFWwindow *window, int screenWidth, int screenHeight) {glViewport(0, 0, screenWidth, screenHeight);}