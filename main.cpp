#include "SceneManager.h"  
#include "DefaultScene.hpp"  
#include "ModelRenderer.hpp"  

int main() {
    auto* sceneManager = new SceneManager();


    sceneManager->SetCurrentScene(new DefaultScene("Main"));
    sceneManager->Init();

    std::vector<Camera*> cameras = { sceneManager->GetGlobalCamera() };
    auto* node = new ModelRenderer("models/bunny/scene.gltf", cameras);

    sceneManager->AddNode(node);

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
