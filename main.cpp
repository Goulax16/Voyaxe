#include "SceneManager.h"  
#include "DefaultScene.hpp"  
#include "ModelRenderer.hpp"  

int main() {
    auto sceneManager = std::make_unique<SceneManager>();

    auto mainScene = std::make_shared<DefaultScene>("Main");
    sceneManager->SetCurrentScene(mainScene);

    sceneManager->Init();

    auto globalCamera = sceneManager->GetSharedGlobalCamera();
    std::vector<std::shared_ptr<Camera>> cameras = { globalCamera };

    auto modelRenderer = std::make_shared<ModelRenderer>("models/bunny/scene.gltf", cameras);

    sceneManager->AddNode(modelRenderer);
    
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(sceneManager->GetWindow())) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        sceneManager->Update(deltaTime);
    }

    return 0;
}
