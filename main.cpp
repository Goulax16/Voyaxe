#include "SceneManager.h"  
#include "DefaultScene.hpp"  
#include "ModelRenderer.hpp"

int main() {
    auto sceneManager = std::make_unique<SceneManager>();

    auto mainScene = std::make_shared<DefaultScene>("Main");
    sceneManager->SetCurrentScene(mainScene);

    sceneManager->Init();
    sceneManager->EnableWindowVSync(false);

    auto globalCamera = sceneManager->GetSharedGlobalCamera();
    std::vector<std::shared_ptr<Camera>> cameras = { globalCamera };

    auto modelRenderer = std::make_shared<ModelRenderer>("models/bunny/scene.gltf", cameras);
    modelRenderer->ChangeColor({0.24f, 0.12f, 0.63f});

    auto modelRenderer2 = std::make_shared<ModelRenderer>("models/bunny/scene.gltf", cameras);
    modelRenderer2->ChangeColor({0.63, 0.24f, 0.12});
    modelRenderer2->name = "Hello";
    modelRenderer2->transform.Translate(1.0f, 0.0f, -2.0f);

    sceneManager->AddNode(modelRenderer);
    sceneManager->AddNode(modelRenderer2);
    
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(sceneManager->GetWindow())) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        modelRenderer->transform.Rotate(45.0f * deltaTime, 0.0f, 1.0f, 0.0f);

        sceneManager->Update(deltaTime);
    }

    return 0;
}
