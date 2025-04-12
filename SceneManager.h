#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

#include "Node.h"
#include "Window.h"
#include "Scene.hpp"

class SceneManager {
private:
    std::unique_ptr<Window> window;
    std::shared_ptr<Scene> currentScene;

public:
    SceneManager();
    ~SceneManager() = default;

    void SetCurrentScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Scene> GetCurrentScene() const { return currentScene; }

    void Init();
    void Update(float dt);

    void AddNode(std::shared_ptr<Node> node);

    std::shared_ptr<Node> GetNode(const std::string& name) const;
    std::vector<std::shared_ptr<Node>> GetNodes() const;

    GLFWwindow* GetWindow() const { return window ? window->GetNativeWindow() : nullptr; }
    Camera* GetGlobalCamera() const { return window ? &window->GetGlobalCamera() : nullptr; }
    std::shared_ptr<Camera> GetSharedGlobalCamera() const { return window ? window->GetCamera() : nullptr; }
};