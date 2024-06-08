#include "DemoLevel.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameCommands.h"
#include "Displays.h"
#include "LevelLoader.h"
#include "Wall.h"
#include <ServiceLocator.h>

DemoLevel::DemoLevel(dae::Scene* pScene)
{
	std::unique_ptr<dae::Subject<game::EventType>> sceneStartSubject = std::make_unique<dae::Subject<game::EventType>>();
	dae::ServiceLocator::GetSoundSystem().PlayMusic("../Data/Resources/Sounds/WalkTheme.mp3", 10, 10000);

	auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 18);
	auto fpsObject = std::make_unique<dae::GameObject>();
	fpsObject->AddComponent<dae::TextureComponent>();
	fpsObject->AddComponent<dae::TextComponent>("Empty", std::move(fpsFont));
	fpsObject->AddComponent<dae::FPSComponent>();
	fpsObject->SetLocalPosition(40, 10);
	pScene->Add(std::move(fpsObject));

	auto highScoreFont = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 25);
	auto highScoreObject = std::make_unique<dae::GameObject>();
	highScoreObject->AddComponent<dae::TextureComponent>();
	highScoreObject->AddComponent<dae::TextComponent>("Empty", std::move(highScoreFont));
	highScoreObject->AddComponent<game::HighScoreComponent>();
	highScoreObject->SetLocalPosition(500, 10);
	pScene->Add(std::move(highScoreObject));

	//===============================================================================================================
	//2 PLAYERS (1 KEYBOARD, 1 CONTROLLER)
	//===============================================================================================================
	auto font = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto player1ControlsText = std::make_unique<dae::GameObject>();
	player1ControlsText->AddComponent<dae::TextureComponent>();
	player1ControlsText->AddComponent<dae::TextComponent>("Use D-Pad or WASD to move Dig-Dug", std::move(font));
	player1ControlsText->SetLocalPosition(100, 30);
	pScene->Add(std::move(player1ControlsText));

	font = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	auto player1ControlsText2 = std::make_unique<dae::GameObject>();
	player1ControlsText2->AddComponent<dae::TextureComponent>();
	player1ControlsText2->AddComponent<dae::TextComponent>("Use A or SPACE to pump", std::move(font));
	player1ControlsText2->SetLocalPosition(86, 45);
	pScene->Add(std::move(player1ControlsText2));

	//font = dae::ResourceManager::GetInstance().LoadFont("upheavtt.ttf", 14);
	//auto player2ControlsText = std::make_unique<dae::GameObject>();
	//player2ControlsText->AddComponent<dae::TextureComponent>();
	//player2ControlsText->AddComponent<dae::TextComponent>("Use WASD to move Pooka, P to add points, L to lose lives", std::move(font));
	//player2ControlsText->SetLocalPosition(208, 80);
	//pScene->Add(std::move(player2ControlsText));

	//TEST WALL
	//auto wallObject = std::make_unique<dae::GameObject>();
	//wallObject->AddComponent<dae::TextureComponent>();
	//wallObject->AddComponent<game::WallComponent>(32.f);
	//wallObject->SetLocalPosition(300, 400);
	//pScene->Add(std::move(wallObject));

	game::LevelCreator::GetInstance().m_MapOffset = glm::vec3{ 160.f, 64.f, 0.f };
	game::LevelCreator::GetInstance().CreateLevel(L"../Data/level1.json", pScene, sceneStartSubject.get());

	// START GAME
	sceneStartSubject->Notify(game::EventType::START_GAME);
}