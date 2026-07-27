#pragma once

#include <GLFW/glfw3.h>

class Time {
public:
    float deltaTime = 0, currentFrame = 0, lastFrame = 0;

    void updateDeltaTime() {
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
};

extern Time gameTime;