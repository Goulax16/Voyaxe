#include "SceneManager.h"
#include "DefaultScene.hpp"
#include "ModelRenderer.hpp"

int main() {
    auto* sceneManager = new SceneManager();

    // Correct the initialization of ModelRenderer
    sceneManager->SetCurrentScene(new DefaultScene("Main"));
    sceneManager->Init();

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(sceneManager->GetWindow())) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sceneManager->Update(deltaTime);
    }

    delete sceneManager;

    return 0;
}