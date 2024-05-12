#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
//#include "Scene.h"
//#include "ResourceManager.h"
//#include "InputManager.h"
//#include "GameCommands.h"
//#include "Displays.h"
//#include "Event.h"
//#include "GameComponents.h"
#include "ServiceLocator.h"
//#include "Wall.h"
#include "DemoLevel.h"

void load();
int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}

//using namespace dae;
//using namespace game;

void load()
{
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	DemoLevel demoLevel{ &scene };
}