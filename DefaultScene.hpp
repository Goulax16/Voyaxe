#include "Scene.hpp"

#include "SceneManager.h"

class DefaultScene : public Scene 
{
public:
	DefaultScene(std::string sceneName) : Scene(sceneName) {
		
	}
};