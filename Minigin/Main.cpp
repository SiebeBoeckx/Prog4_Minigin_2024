#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"

using namespace dae;
void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	RenderComponent* backgroundRender = &go->AddComponent<RenderComponent>();
	backgroundRender->SetTexture("background.tga");
	go->SetPosition(320, 240);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	RenderComponent* logoRender = &go->AddComponent<RenderComponent>();
	logoRender->SetTexture("logo.tga");
	go->SetPosition(320, 170);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<GameObject>();
	&to->AddComponent<RenderComponent>();
	&to->AddComponent<TextComponent>("Programming 4 Assignment", font);
	to->SetPosition(320, 30);
	scene.Add(to);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}