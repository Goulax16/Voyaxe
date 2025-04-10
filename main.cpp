#include "SceneManager.h"  
#include "DefaultScene.hpp"  
#include "ModelRenderer.hpp"  

int main() {
    auto* sceneManager = new SceneManager();

    sceneManager->SetCurrentScene(new Scene("Main"));
    sceneManager->Init();

    std::vector<Camera*> cameras = { sceneManager->GetGlobalCamera() };
    auto* modelRenderer = new ModelRenderer("models/bunny/scene.gltf", cameras);

    sceneManager->AddNode(modelRenderer);
    
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(sceneManager->GetWindow())) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sceneManager->Update(deltaTime);
    }

    delete sceneManager;

    return 0;
}
