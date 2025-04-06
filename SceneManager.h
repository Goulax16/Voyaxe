#pragma once

#include "Node.h"
#include "Window.h"
#include "Scene.hpp"

class Scene; // Forward Decl

class SceneManager
{
private:
    Window* window;
    Scene* currentScene;

public:
    SceneManager();
    ~SceneManager();

    Scene* GetCurrentScene() { return currentScene; }
    void SetCurrentScene(Scene* scene);

    void Init();
    void Update(float dt);

    void AddNode(Node* node);

    Node* GetNode(std::string name) { return currentScene->GetNode(name); }
    std::vector<Node*> GetNodes() { return currentScene->GetNodes(); }

    GLFWwindow* GetWindow() { return window->GetNativeWindow(); }
    Camera* GetGlobalCamera() { return &window->GetGlobalCamera(); }

    void BeginViewportRender() { if (window) window->BeginViewportRender(); }
    void EndViewportRender() { if (window) window->EndViewportRender(); }
    unsigned int GetViewportTexture() const { return window ? window->GetViewportTexture() : 0; }

    glm::vec2 GetViewportSize() const { return window ? window->GetViewportSize() : glm::vec2(0.0f); }
    void SetViewportSize(const glm::vec2& size) { if (window) window->SetViewportSize(size); }
};