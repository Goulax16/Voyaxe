#pragma once

#include <GLFW/glfw3.h>
#include "util.h"

/* UTILITY CLASS */
class Time
{
private:
    static vFLOAT& lastFrameTime() {
        static vFLOAT value = 0.0f;
        return value;
    }

    static vFLOAT& deltaTime() {
        static vFLOAT value = 0.0f;
        return value;
    }

public:
    static void Update() {
        vFLOAT currentFrameTime = static_cast<vFLOAT>(glfwGetTime());
        deltaTime() = currentFrameTime - lastFrameTime();
        lastFrameTime() = currentFrameTime;
    }

    static vFLOAT getDeltaTime() { return deltaTime(); }
};
