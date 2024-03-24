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

using namespace dae;
void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

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
	
	//player 1
	int playerIdx = InputManager::GetInstance().AddPlayer();
	auto player1 = std::make_unique<GameObject>();
	TextureComponent* texture = &player1->AddComponent<TextureComponent>();
	texture->SetTexture("Resources/Sprites/Walking0.png");
	player1.get()->SetLocalPosition(100, 400);

	auto moveUp = std::make_unique<MoveCommand>(player1.get(), glm::vec2{0, -1}, 50.f);
	auto moveDown = std::make_unique<MoveCommand>(player1.get(), glm::vec2{ 0, 1 }, 50.f);
	auto moveRight = std::make_unique<MoveCommand>(player1.get(), glm::vec2{ 1, 0 }, 50.f);
	auto moveLeft = std::make_unique<MoveCommand>(player1.get(), glm::vec2{ -1, 0 }, 50.f);

	// UP
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadUp, std::move(moveUp), playerIdx, InputManager::KeyState::Pressed);
	// DOWN
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadDown, std::move(moveDown), playerIdx, InputManager::KeyState::Pressed);
	// RIGHT
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadRight, std::move(moveRight), playerIdx, InputManager::KeyState::Pressed);
	// LEFT
	InputManager::GetInstance().AddCommand(XBox360Controller::Button::DPadLeft, std::move(moveLeft), playerIdx, InputManager::KeyState::Pressed);

	scene.Add(std::move(player1));

	//player 2
	playerIdx = InputManager::GetInstance().AddPlayer();
	auto player2 = std::make_unique<GameObject>();
	texture = &player2->AddComponent<TextureComponent>();
	texture->SetTexture("Resources/Sprites/Pooka0.png");
	player2.get()->SetLocalPosition(50, 400);
	
	moveUp = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ 0, -1 }, 100.f);
	moveDown = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ 0, 1 }, 100.f);
	moveRight = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ 1, 0 }, 100.f);
	moveLeft = std::make_unique<MoveCommand>(player2.get(), glm::vec2{ -1, 0 }, 100.f);

	// UP
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_W, std::move(moveUp), playerIdx, InputManager::KeyState::Pressed);
	// DOWN
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_S, std::move(moveDown), playerIdx, InputManager::KeyState::Pressed);
	// RIGHT
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_D, std::move(moveRight), playerIdx, InputManager::KeyState::Pressed);
	// LEFT
	InputManager::GetInstance().AddCommand(SDL_SCANCODE_A, std::move(moveLeft), playerIdx, InputManager::KeyState::Pressed);
	
	scene.Add(std::move(player2));
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}