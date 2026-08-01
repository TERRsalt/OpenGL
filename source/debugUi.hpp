#pragma once

#include <GLFW/glfw3.h>

namespace debugUi {
    void init(GLFWwindow *window);
    void remove();

    void debug();
}