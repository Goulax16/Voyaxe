#include "SceneManager.h"

SceneManager::SceneManager() : window(nullptr), currentScene(nullptr) {}

void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene) {
    if (scene && scene != currentScene) {
        currentScene = scene;
    }
}

void SceneManager::Init() {
    window = std::make_unique<Window>(1280, 720, "Hello");
    if (!window->Initialize()) {
        throw std::runtime_error("Failed to initialize the window.");
    }
}

void SceneManager::Update(float dt) {
    if (currentScene && window) {
        currentScene->Update(dt);
        window->Update();
    }
}

void SceneManager::AddNode(std::shared_ptr<Node> node) {
    if (!node || !currentScene) return;

    currentScene->AddNode(node);
    window->AddRenderCallback([node]() { node->Render(); });
    window->m_nodeList.push_back(node);
}

std::shared_ptr<Node> SceneManager::GetNode(const std::string& name) const {
    if (currentScene) {
        return currentScene->GetNode(name);
    }
    throw std::runtime_error("No current scene set.");
}

std::vector<std::shared_ptr<Node>> SceneManager::GetNodes() const {
    if (currentScene) {
        return currentScene->GetNodes();
    }
    return {};
}