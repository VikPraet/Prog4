#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <windows.h>
#include <Xinput.h>

#include "Settings.h"
#include "Minigin.h"
#include "FpsComponent.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "RotationComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "DisplayComponentHealth.h"
#include "DisplayComponentScore.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// -- background --
	auto backGround = std::make_shared<dae::GameObject>();
	// Transform
	backGround->AddComponent<TransformComponent>(backGround);
	backGround->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, Settings::window_height / 2);
	// Render
	backGround->AddComponent<RenderComponent>(backGround);
	backGround->GetComponent<RenderComponent>()->SetTexture("background.tga");
	// Add to scene
	scene.Add(backGround);

	// -- logo --
	auto logo = std::make_shared<dae::GameObject>();
	// Transform
	logo->AddComponent<TransformComponent>(logo);
	logo->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 230);
	// Render
	logo->AddComponent<RenderComponent>(logo);
	logo->GetComponent<RenderComponent>()->SetTexture("logo.tga");
	// Add to scene
	scene.Add(logo);

	// -- text (assignment) --
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textA = std::make_shared<dae::GameObject>();
	// Transform
	textA->AddComponent<TransformComponent>(textA);
	textA->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 50);
	// Text
	textA->AddComponent<TextComponent>(textA, "Programming 4 Assignment", font);
	// Render
	textA->AddComponent<RenderComponent>(textA);
	// Add to scene
	scene.Add(textA);

	// -- text (D-pad) --
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf",20);
	auto textDPad = std::make_shared<dae::GameObject>();
	// Transform
	textDPad->AddComponent<TransformComponent>(textDPad);
	textDPad->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 130);
	// Text
	textDPad->AddComponent<TextComponent>(textDPad, "use the D-Pad to move the fighter, A to damage, X to increase score", smallFont);
	// Render
	textDPad->AddComponent<RenderComponent>(textDPad);
	// Add to scene
	scene.Add(textDPad);

	// -- text (WASD) --
	auto textWASD = std::make_shared<dae::GameObject>();
	// Transform
	textWASD->AddComponent<TransformComponent>(textWASD);
	textWASD->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 160);
	// Text
	textWASD->AddComponent<TextComponent>(textWASD, "use WASD to move the boss, F to damage, X to increase score", smallFont);
	// Render
	textWASD->AddComponent<RenderComponent>(textWASD);
	// Add to scene
	scene.Add(textWASD);

	// -- FPS counter --
	auto fpsCounter = std::make_shared<dae::GameObject>();
	// Transform
	fpsCounter->AddComponent<TransformComponent>(fpsCounter);
	fpsCounter->GetComponent<TransformComponent>()->SetWorldPosition(100, 450);
	// Fps
	fpsCounter->AddComponent<FpsComponent>(fpsCounter);
	// Text
	fpsCounter->AddComponent<TextComponent>(fpsCounter, font);
	// Render
	fpsCounter->AddComponent<RenderComponent>(fpsCounter);
	// Add to scene
	scene.Add(fpsCounter);

	// -- Fighter --
	auto fighter = std::make_shared<dae::GameObject>();
	// Transform
	fighter->AddComponent<TransformComponent>(fighter);
	fighter->GetComponent<TransformComponent>()->SetWorldPosition(200, 300);
	fighter->GetComponent<TransformComponent>()->SetScale(2);
	// Render
	fighter->AddComponent<RenderComponent>(fighter);
	fighter->GetComponent<RenderComponent>()->SetTexture("galaga-fighter.png");
	// Health
	fighter->AddComponent<dae::HealthComponent>(fighter, 3.f);
	// Score
	fighter->AddComponent<dae::ScoreComponent>(fighter);
	// Add to scene
	scene.Add(fighter);

	// -- Boss --
	auto boss = std::make_shared<dae::GameObject>();
	// Transform
	boss->AddComponent<TransformComponent>(boss);
	boss->GetComponent<TransformComponent>()->SetWorldPosition(250, 300);
	boss->GetComponent<TransformComponent>()->SetScale(2);
	// Render
	boss->AddComponent<RenderComponent>(boss);
	boss->GetComponent<RenderComponent>()->SetTexture("galaga-boss.png");
	// Health
	boss->AddComponent<dae::HealthComponent>(boss, 3.f);
	// Score
	boss->AddComponent<dae::ScoreComponent>(boss);
	// Add to scene
	scene.Add(boss);

	// -- lives display fighter --
	auto displayFighterHealth = std::make_shared<dae::GameObject>();
	// Transform
	displayFighterHealth->AddComponent<TransformComponent>(displayFighterHealth);
	displayFighterHealth->GetComponent<TransformComponent>()->SetWorldPosition(100, 250);
	// Text
	displayFighterHealth->AddComponent<TextComponent>(displayFighterHealth, smallFont);
	// Render
	displayFighterHealth->AddComponent<RenderComponent>(displayFighterHealth);
	// LivesDisplay
	displayFighterHealth->AddComponent<dae::DisplayComponentHealth>(displayFighterHealth);
	// Add to scene
	scene.Add(displayFighterHealth);

	// -- lives display boss --
	auto displayBossHealth = std::make_shared<dae::GameObject>();
	// Transform
	displayBossHealth->AddComponent<TransformComponent>(displayBossHealth);
	displayBossHealth->GetComponent<TransformComponent>()->SetWorldPosition(100, 300);
	// Text
	displayBossHealth->AddComponent<TextComponent>(displayBossHealth, smallFont);
	// Render
	displayBossHealth->AddComponent<RenderComponent>(displayBossHealth);
	// LivesDisplay
	displayBossHealth->AddComponent<dae::DisplayComponentHealth>(displayBossHealth);
	// Add to scene
	scene.Add(displayBossHealth);

	// -- score display fighter --
	auto displayFighterScore = std::make_shared<dae::GameObject>();
	// Transform
	displayFighterScore->AddComponent<TransformComponent>(displayFighterScore);
	displayFighterScore->GetComponent<TransformComponent>()->SetWorldPosition(100, 270);
	// Text
	displayFighterScore->AddComponent<TextComponent>(displayFighterScore, smallFont);
	// Render
	displayFighterScore->AddComponent<RenderComponent>(displayFighterScore);
	// LivesDisplay
	displayFighterScore->AddComponent<dae::DisplayComponentScore>(displayFighterScore);
	// Add to scene
	scene.Add(displayFighterScore);

	// -- score display boss --
	auto displayBossScore = std::make_shared<dae::GameObject>();
	// Transform
	displayBossScore->AddComponent<TransformComponent>(displayBossScore);
	displayBossScore->GetComponent<TransformComponent>()->SetWorldPosition(100, 320);
	// Text
	displayBossScore->AddComponent<TextComponent>(displayBossScore, smallFont);
	// Render
	displayBossScore->AddComponent<RenderComponent>(displayBossScore);
	// LivesDisplay
	displayBossScore->AddComponent<dae::DisplayComponentScore>(displayBossScore);
	// Add to scene
	scene.Add(displayBossScore);

	fighter->GetComponent<dae::HealthComponent>()->AddObserver(displayFighterHealth->GetComponent<dae::DisplayComponentHealth>());
	fighter->GetComponent<dae::ScoreComponent>()->AddObserver(displayFighterScore->GetComponent<dae::DisplayComponentScore>());

	boss->GetComponent<dae::HealthComponent>()->AddObserver(displayBossHealth->GetComponent<dae::DisplayComponentHealth>());
	boss->GetComponent<dae::ScoreComponent>()->AddObserver(displayBossScore->GetComponent<dae::DisplayComponentScore>());

	// Input
	// add controllers
	dae::InputManager::GetInstance().AddController();

	// fighter
	float speed{50};
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_LEFT), std::make_unique<dae::MoveCommand>(fighter, glm::vec2{-speed, 0}), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_RIGHT), std::make_unique<dae::MoveCommand>(fighter, glm::vec2{ speed, 0}), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_UP), std::make_unique<dae::MoveCommand>(fighter, glm::vec2{0, -speed }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_DOWN), std::make_unique<dae::MoveCommand>(fighter, glm::vec2{0, speed }), dae::InputActionType::Continuous);

	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_A), std::make_unique<dae::DamageCommand>(fighter, 1.f), dae::InputActionType::OnPressed);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_X), std::make_unique<dae::ScoreGainCommand>(fighter, 100), dae::InputActionType::OnPressed);

	// boss
	speed = 100;
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_a), std::make_unique<dae::MoveCommand>(boss, glm::vec2{ -speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_d), std::make_unique<dae::MoveCommand>(boss, glm::vec2{ speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_w), std::make_unique<dae::MoveCommand>(boss, glm::vec2{ 0, -speed }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_s), std::make_unique<dae::MoveCommand>(boss, glm::vec2{ 0, speed }), dae::InputActionType::Continuous);

	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_f), std::make_unique<dae::DamageCommand>(boss, 1.f), dae::InputActionType::OnPressed);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_x), std::make_unique<dae::ScoreGainCommand>(boss, 100), dae::InputActionType::OnPressed);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}