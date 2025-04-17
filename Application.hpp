#pragma once

#include "SceneManager.h"
#include <memory>
#include <string>
#include <GLFW/glfw3.h>

namespace Voyaxe {

class Application {
public:
    Application(int width = 1280, int height = 720, const std::string& title = "Voyaxe Application");
    virtual ~Application() = default;

    // Starts the application main loop
    void Run();

protected:
    // Update hook called every frame with delta time
    virtual void OnUpdate(float dt) {}

    // Shutdown hook called after exiting the main loop
    virtual void OnShutdown() {}

    // Must create and return the initial scene for the application
    virtual std::shared_ptr<Scene> CreateScene() = 0;

    // Access to the scene manager
    SceneManager& GetSceneManager() const { return *sceneManager_; }

    // Access to native GLFW window
    GLFWwindow* GetWindow() const { return sceneManager_->GetWindow(); }

private:
    int width_;
    int height_;
    std::string title_;
    std::unique_ptr<SceneManager> sceneManager_;
    float lastFrameTime_;
};

} // namespace Voyaxe
