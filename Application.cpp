#include "Application.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Voyaxe {

Application::Application(int width, int height, const std::string& title)
    : width_(width), height_(height), title_(title), sceneManager_(std::make_unique<SceneManager>()), lastFrameTime_(0.0f)
{
}

void Application::Run() {
    try {
        sceneManager_->Init();
        GLFWwindow* window = sceneManager_->GetWindow();
        if (window) {
            glfwSetWindowSize(window, width_, height_);
            glfwSetWindowTitle(window, title_.c_str());
        }
        // Create and set initial scene
        auto scene = CreateScene();
        sceneManager_->SetCurrentScene(scene);
    } catch (const std::exception& e) {
        std::cerr << "Application initialization failed: " << e.what() << std::endl;
        return;
    }

    lastFrameTime_ = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(sceneManager_->GetWindow())) {
        float currentTime = static_cast<float>(glfwGetTime());
        float dt = currentTime - lastFrameTime_;
        lastFrameTime_ = currentTime;

        OnUpdate(dt);
        sceneManager_->Update(dt);
    }

    OnShutdown();
}

} // namespace Voyaxe
