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
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "InputManager.h"
#include "GameCommands.h"
#include "Displays.h"

using namespace dae;
void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	std::unique_ptr<Subject> sceneStartSubject = std::make_unique<Subject>();

	auto go = std::make_unique<GameObject>();
	TextureComponent* backgroundRender = &go->AddComponent<TextureComponent>();
	backgroundRender->SetTexture("background.tga");
	go->SetLocalPosition(320, 240);
	scene.Add(std::move(go));

	go = std::make_unique<GameObject>();
	TextureComponent* logoRender = &go->AddComponent<TextureComponent>();
	logoRender->SetTexture("logo.tga");
	go->SetLocalPosition(320, 170);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<GameObject>();
	to->AddComponent<TextureComponent>();
	to->AddComponent<TextComponent>("Programming 4 Assignment", std::move(font));
	to->SetLocalPosition(320, 30);
	scene.Add(std::move(to));

	auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	auto fpsObject = std::make_unique<GameObject>();
	fpsObject->AddComponent<TextureComponent>();
	fpsObject->AddComponent<TextComponent>("Empty", std::move(fpsFont));
	fpsObject->AddComponent<FPSComponent>();
	fpsObject->SetLocalPosition(40, 20);
	scene.Add(std::move(fpsObject));

	//===============================================================================================================
	//ENCIRCLING COMPONENT(PARENT CHILD RELATION)
	//===============================================================================================================

	//auto enCircleCenter = std::make_unique<GameObject>();
	//enCircleCenter->SetLocalPosition(100, 300);
	//
	//auto firstEncircleObject = std::make_unique<GameObject>();
	//TextureComponent* texture = &firstEncircleObject->AddComponent<TextureComponent>();
	//texture->SetTexture("Resources/Sprites/Walking0.png");
	//firstEncircleObject->AddComponent<Encircle>(40.f, 2.f);
	//firstEncircleObject->SetParent(enCircleCenter.get());
	//
	//auto secondEncircleObject = std::make_unique<GameObject>();
	//texture = &secondEncircleObject->AddComponent<TextureComponent>();
	//texture->SetTexture("Resources/Sprites/Walking0.png");
	//secondEncircleObject->AddComponent<Encircle>(20.f, -5.f);
	//secondEncircleObject->SetParent(firstEncircleObject.get());
	//
	////Move after parenting, pointers invalid after move
	//scene.Add(std::move(enCircleCenter));
	//scene.Add(std::move(firstEncircleObject));
	//scene.Add(std::move(secondEncircleObject));

	//===============================================================================================================
	//IMGUI AND TRASH THE CACHE
	//===============================================================================================================

	//auto ImGuiObject = std::make_unique<GameObject>();
	//&ImGuiObject->AddComponent<ImGuiComponent>();
	//scene.Add(std::move(ImGuiObject));

	//===============================================================================================================
	//2 CONTROLLERS
	//===============================================================================================================
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto player1ControlsText = std::make_unique<GameObject>();
	player1ControlsText->AddComponent<TextureComponent>();
	player1ControlsText->AddComponent<TextComponent>("Use D-Pad to move Dig-Dug, Y to add points, X to lose lives", std::move(font));
	player1ControlsText->SetLocalPosition(190, 60);
	scene.Add(std::move(player1ControlsText));

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto player2ControlsText = std::make_unique<GameObject>();
	player2ControlsText->AddComponent<TextureComponent>();
	player2ControlsText->AddComponent<TextComponent>("Use WASD to move Pooka, P to add points, L to lose lives", std::move(font));
	player2ControlsText->SetLocalPosition(184, 80);
	scene.Add(std::move(player2ControlsText));
	
	//player 1
	int playerIdx = InputManager::GetInstance().AddPlayer();
	auto player1 = std::make_unique<GameObject>();
	TextureComponent* texture = &player1->AddComponent<TextureComponent>();
	texture->SetTexture("Resources/Sprites/Walking0.png");
	player1.get()->SetLocalPosition(100, 400);
	PlayerComponent* playerComp = &player1->AddComponent<PlayerComponent>(playerIdx);

	auto moveUp = std::make_unique<MoveCommand>(player1.get(), glm::vec2{0, -1}, 50.f);
	auto moveDown = std::make_unique<MoveCommand>(player1.get(), glm::vec2{ 0, 1 }, 50.f);
	auto moveRight = std::make_unique<MoveCommand>(player1.get(), glm::vec2{ 1, 0 }, 50.f);
	auto moveLeft = std::make_unique<MoveCommand>(player1.get(), glm::vec2{ -1, 0 }, 50.f);

	auto addPoints = std::make_unique<AddPointsCommand>(player1.get(), 50);
	auto loseLife = std::make_unique<LoseLifeCommand>(player1.get());

	// UP
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadUp, std::move(moveUp), playerIdx, InputManager::KeyState::Pressed);
	// DOWN
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadDown, std::move(moveDown), playerIdx, InputManager::KeyState::Pressed);
	// RIGHT
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadRight, std::move(moveRight), playerIdx, InputManager::KeyState::Pressed);
	// LEFT
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadLeft, std::move(moveLeft), playerIdx, InputManager::KeyState::Pressed);
	// ADD POINTS
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::ButtonY, std::move(addPoints), playerIdx, InputManager::KeyState::Down);
	// LOSE LIFE
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::ButtonX, std::move(loseLife), playerIdx, InputManager::KeyState::Down);

	scene.Add(std::move(player1));

	// LIVES DISPLAY PLAYER 1
	auto displayFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto livesDisplayObject = std::make_unique<GameObject>();
	livesDisplayObject->AddComponent<TextureComponent>();
	livesDisplayObject->AddComponent<TextComponent>("Empty", std::move(displayFont));
	livesDisplayObject->AddComponent<LivesDisplayComponent>(playerComp);

	playerComp->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());
	sceneStartSubject->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());

	livesDisplayObject->SetLocalPosition(40, 100);
	scene.Add(std::move(livesDisplayObject));

	// SCORE DISPLAY PLAYER 1
	displayFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto scoreDisplayObject = std::make_unique<GameObject>();
	scoreDisplayObject->AddComponent<TextureComponent>();
	scoreDisplayObject->AddComponent<TextComponent>("Empty", std::move(displayFont));
	scoreDisplayObject->AddComponent<ScoreDisplayComponent>(playerComp);

	playerComp->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	sceneStartSubject->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());

	scoreDisplayObject->SetLocalPosition(40, 120);
	scene.Add(std::move(scoreDisplayObject));


	//player 2
	playerIdx = InputManager::GetInstance().AddPlayer();
	auto player2 = std::make_unique<GameObject>();
	texture = &player2->AddComponent<TextureComponent>();
	texture->SetTexture("Resources/Sprites/Pooka0.png");
	player2.get()->SetLocalPosition(50, 400);
	playerComp = &player2->AddComponent<PlayerComponent>(playerIdx);
	
	moveUp = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ 0, -1 }, 100.f);
	moveDown = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ 0, 1 }, 100.f);
	moveRight = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ 1, 0 }, 100.f);
	moveLeft = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ -1, 0 }, 100.f);

	addPoints = std::make_unique<AddPointsCommand>(player2.get(), 50);
	loseLife = std::make_unique<LoseLifeCommand>(player2.get());

	// UP
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_W, std::move(moveUp), playerIdx, InputManager::KeyState::Pressed);
	// DOWN
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_S, std::move(moveDown), playerIdx, InputManager::KeyState::Pressed);
	// RIGHT
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_D, std::move(moveRight), playerIdx, InputManager::KeyState::Pressed);
	// LEFT
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_A, std::move(moveLeft), playerIdx, InputManager::KeyState::Pressed);

	// ADD POINTS
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_P, std::move(addPoints), playerIdx, InputManager::KeyState::Down);
	// LOSE LIFE
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_L, std::move(loseLife), playerIdx, InputManager::KeyState::Down);
	
	scene.Add(std::move(player2));

	// LIVES DISPLAY PLAYER 2
	displayFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	livesDisplayObject = std::make_unique<GameObject>();
	livesDisplayObject->AddComponent<TextureComponent>();
	livesDisplayObject->AddComponent<TextComponent>("Empty", std::move(displayFont));
	livesDisplayObject->AddComponent<LivesDisplayComponent>(playerComp);

	playerComp->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());
	sceneStartSubject->AddObserver(livesDisplayObject->GetComponent<LivesDisplayComponent>());

	livesDisplayObject->SetLocalPosition(40, 140);
	scene.Add(std::move(livesDisplayObject));

	// SCORE DISPLAY PLAYER 2
	displayFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	scoreDisplayObject = std::make_unique<GameObject>();
	scoreDisplayObject->AddComponent<TextureComponent>();
	scoreDisplayObject->AddComponent<TextComponent>("Empty", std::move(displayFont));
	scoreDisplayObject->AddComponent<ScoreDisplayComponent>(playerComp);

	playerComp->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());
	sceneStartSubject->AddObserver(scoreDisplayObject->GetComponent<ScoreDisplayComponent>());

	scoreDisplayObject->SetLocalPosition(40, 160);
	scene.Add(std::move(scoreDisplayObject));


	// START GAME
	sceneStartSubject->Notify(EventType::START_GAME);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}