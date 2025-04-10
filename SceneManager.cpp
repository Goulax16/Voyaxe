#include "SceneManager.h"

#include "IRenderable.hpp"

SceneManager::SceneManager() : currentScene(nullptr), window(nullptr) {
}

SceneManager::~SceneManager() {
    if (window) {
        delete window;
        window = nullptr;
    }
}

void SceneManager::SetCurrentScene(Scene* scene) {
    if (currentScene != scene) {
        currentScene = scene;
    }
}

void SceneManager::Init() {
    window = new Window(1280, 720, "Hello");
    window->Initialize();
}

void SceneManager::Update(float dt) {
    if (currentScene != nullptr && window != nullptr) {
        currentScene->Update(dt);
        window->Update();
    }
}

void SceneManager::AddNode(Node* node) {
    if (node != nullptr && currentScene != nullptr) {
        currentScene->AddNode(node);

        window->AddRenderCallback([this, node]() {
            node->Render();
            });
    }
}