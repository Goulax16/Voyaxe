#include "Application.hpp"  
#include "SceneManager.h"  
#include "DefaultScene.hpp"  
#include "ModelRenderer.hpp"  

class EngineTest : public Voyaxe::Application  
{  
protected:
   std::shared_ptr<Scene> CreateScene() override {  
       auto mainScene = std::make_shared<DefaultScene>("Main");  

       auto globalCamera = GetSceneManager().GetSharedGlobalCamera();  
       std::vector<std::shared_ptr<Camera>> cameras = { globalCamera };  

       auto modelRenderer = std::make_shared<ModelRenderer>("models/bunny/scene.gltf", cameras);  
       modelRenderer->ChangeColor({1.0f, 0.92f, 0.70f});  

       auto modelRenderer2 = std::make_shared<ModelRenderer>("models/cube/test_cube.gltf", cameras);  
       modelRenderer2->ChangeColor({0.12, 0.95f, 0.87});
       modelRenderer2->name = "Hello";
       modelRenderer2->transform.Translate(1.0f, 0.0f, -2.0f);

       mainScene->AddNode(modelRenderer);
       mainScene->AddNode(modelRenderer2);

       return mainScene;
   }

   void OnUpdate(float dt) override {
       auto modelRenderer = GetSceneManager().GetNode("ModelRenderer");
       if (modelRenderer) {
           auto modelRendererBridge = dynamic_cast<ModelRenderer*>(modelRenderer.get());
           modelRendererBridge->transform.Rotate(45.0f * dt, 0.0f, 1.0f, 0.0f);
       }
   }
};

int main() {
   EngineTest app;
   app.Run();
   return 0;  
}
