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
    window = new Window(800, 600, "Hello");
    window->Initialize();
}

void SceneManager::Update(float dt) {
    if (currentScene != nullptr && window != nullptr) {
        currentScene->Update(dt);

        BeginViewportRender();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto nodes = currentScene->GetNodes();
        for (auto node : nodes) {
            if (auto renderable = dynamic_cast<IRenderable*>(node)) {
                renderable->Render();
            }
        }

        EndViewportRender();

        window->Update();
    }
}

void SceneManager::AddNode(Node* node) {
    if (node != nullptr && currentScene != nullptr) {
        currentScene->AddNode(node);
    }
}