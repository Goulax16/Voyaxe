#include "SceneManager.h"

SceneManager::SceneManager() : window(nullptr), currentScene(nullptr) {}

void SceneManager::SetCurrentScene(std::shared_ptr<Scene> scene) {
    if (scene && scene != currentScene) {
        if (currentScene) {
            try {
                currentScene->clear();
                window->RemoveAllRenderCallbacks();
                window->m_nodeList.clear();
            }
            catch (const std::exception& e) {
                // Log the error or handle it appropriately
                std::cerr << "Error clearing scene: " << e.what() << std::endl;
            }
        }
        currentScene = scene;
        currentScene->Init();
        for (const auto& node : currentScene->GetNodes()) {
            window->AddRenderCallback([node]() { node->Render(); });
            window->m_nodeList.push_back(node);
        }
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
        currentScene->Render();
        window->Update();
    }
}

void SceneManager::AddNode(std::shared_ptr<Node> node) {
    if (!node || !currentScene) return;

    try {
        currentScene->AddNode(node);
        window->AddRenderCallback([node]() { node->Render(); });
        window->m_nodeList.push_back(node);
        std::cout << "Node added successfully: " << node->name << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in SceneManager::AddNode: " << e.what() << std::endl;
        throw;
    }
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