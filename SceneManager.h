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

    void BeginViewportRender() const { if (window) window->BeginViewportRender(); }
    void EndViewportRender() const { if (window) window->EndViewportRender(); }
    unsigned int GetViewportTexture() const { return window ? window->GetViewportTexture() : 0; }

    glm::vec2 GetViewportSize() const { return window ? window->GetViewportSize() : glm::vec2(0.0f); }
    void SetViewportSize(const glm::vec2& size) { if (window) window->SetViewportSize(size); }
};