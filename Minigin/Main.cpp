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
	TextureComponent* backgroundRender = &go->AddComponent<TextureComponent>();
	backgroundRender->SetTexture("background.tga");
	go->SetLocalPosition(320, 240);
	scene.Add(go);

	go = std::make_shared<GameObject>();
	TextureComponent* logoRender = &go->AddComponent<TextureComponent>();
	logoRender->SetTexture("logo.tga");
	go->SetLocalPosition(320, 170);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<GameObject>();
	to->AddComponent<TextureComponent>();
	to->AddComponent<TextComponent>("Programming 4 Assignment", font);
	to->SetLocalPosition(320, 30);
	scene.Add(to);

	auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent<TextureComponent>();
	fpsObject->AddComponent<TextComponent>("Empty", fpsFont);
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(40, 20);
	scene.Add(fpsObject);

	auto enCircleCenter = std::make_shared<GameObject>();
	enCircleCenter->SetLocalPosition(100, 300);
	scene.Add(enCircleCenter);

	auto firstEncircleObject = std::make_shared<GameObject>();
	TextureComponent* texture = &firstEncircleObject->AddComponent<TextureComponent>();
	texture->SetTexture("Resources/Sprites/Walking0.png");
	firstEncircleObject->AddComponent<Encircle>(40.f, 2.f, true);
	firstEncircleObject->SetParent(enCircleCenter.get());
	scene.Add(firstEncircleObject);

	auto secondEncircleObject = std::make_shared<GameObject>();
	texture = &secondEncircleObject->AddComponent<TextureComponent>();
	texture->SetTexture("Resources/Sprites/Walking0.png");
	secondEncircleObject->AddComponent<Encircle>(20.f, 1.f, false);
	secondEncircleObject->SetParent(firstEncircleObject.get());
	scene.Add(secondEncircleObject);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}