#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <Windows.h>
#include <Xinput.h>

#include "Minigin.h"

#include "Settings.h"
#include "FpsComponent.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "DisplayComponentHealth.h"
#include "DisplayComponentScore.h"


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// -- background --
	auto backGround = std::make_unique<dae::GameObject>();
	// Transform
	backGround->AddComponent<TransformComponent>(backGround.get());
	backGround->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, Settings::window_height / 2);
	// Render
	backGround->AddComponent<RenderComponent>(backGround.get());
	backGround->GetComponent<RenderComponent>()->SetTexture("background.tga");


	// -- logo --
	auto logo = std::make_unique<dae::GameObject>();
	// Transform
	logo->AddComponent<TransformComponent>(logo.get());
	logo->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 230);
	// Render
	logo->AddComponent<RenderComponent>(logo.get());
	logo->GetComponent<RenderComponent>()->SetTexture("logo.tga");


	// -- text (assignment) --
	auto textA = std::make_unique<dae::GameObject>();
	// Transform
	textA->AddComponent<TransformComponent>(textA.get());
	textA->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 50);
	// Text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textA->AddComponent<TextComponent>(textA.get(), "Programming 4 Assignment", std::move(font));
	// Render
	textA->AddComponent<RenderComponent>(textA.get());


	// -- text (D-pad) --
	auto textDPad = std::make_unique<dae::GameObject>();
	// Transform
	textDPad->AddComponent<TransformComponent>(textDPad.get());
	textDPad->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 130);
	// Text
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	textDPad->AddComponent<TextComponent>(textDPad.get(), "use the D-Pad to move the fighter, A to damage, X to increase score", std::move(smallFont));
	// Render
	textDPad->AddComponent<RenderComponent>(textDPad.get());


	// -- text (WASD) --
	auto textWASD = std::make_unique<dae::GameObject>();
	// Transform
	textWASD->AddComponent<TransformComponent>(textWASD.get());
	textWASD->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 160);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	textWASD->AddComponent<TextComponent>(textWASD.get(), "use WASD to move the boss, F to damage, X to increase score", std::move(smallFont));
	// Render
	textWASD->AddComponent<RenderComponent>(textWASD.get());


	// -- FPS counter --
	auto fpsCounter = std::make_unique<dae::GameObject>();
	// Transform
	fpsCounter->AddComponent<TransformComponent>(fpsCounter.get());
	fpsCounter->GetComponent<TransformComponent>()->SetWorldPosition(100, 450);
	// Fps
	fpsCounter->AddComponent<FpsComponent>(fpsCounter.get());
	// Text
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	fpsCounter->AddComponent<TextComponent>(fpsCounter.get(), std::move(font));
	// Render
	fpsCounter->AddComponent<RenderComponent>(fpsCounter.get());


	// -- Fighter --
	auto fighter = std::make_unique<dae::GameObject>();
	// Transform
	fighter->AddComponent<TransformComponent>(fighter.get());
	fighter->GetComponent<TransformComponent>()->SetWorldPosition(200, 300);
	fighter->GetComponent<TransformComponent>()->SetScale(2);
	// Render
	fighter->AddComponent<RenderComponent>(fighter.get());
	fighter->GetComponent<RenderComponent>()->SetTexture("galaga-fighter.png");
	// Health
	fighter->AddComponent<dae::HealthComponent>(fighter.get(), 3.f);
	// Score
	fighter->AddComponent<dae::ScoreComponent>(fighter.get());


	// -- Boss --
	auto boss = std::make_unique<dae::GameObject>();
	// Transform
	boss->AddComponent<TransformComponent>(boss.get());
	boss->GetComponent<TransformComponent>()->SetWorldPosition(250, 300);
	boss->GetComponent<TransformComponent>()->SetScale(2);
	// Render
	boss->AddComponent<RenderComponent>(boss.get());
	boss->GetComponent<RenderComponent>()->SetTexture("galaga-boss.png");
	// Health
	boss->AddComponent<dae::HealthComponent>(boss.get(), 3.f);
	// Score
	boss->AddComponent<dae::ScoreComponent>(boss.get());


	// -- lives display fighter --
	auto displayFighterHealth = std::make_unique<dae::GameObject>();
	// Transform
	displayFighterHealth->AddComponent<TransformComponent>(displayFighterHealth.get());
	displayFighterHealth->GetComponent<TransformComponent>()->SetWorldPosition(100, 250);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayFighterHealth->AddComponent<TextComponent>(displayFighterHealth.get(), std::move(smallFont));
	// Render
	displayFighterHealth->AddComponent<RenderComponent>(displayFighterHealth.get());
	// LivesDisplay
	displayFighterHealth->AddComponent<dae::DisplayComponentHealth>(displayFighterHealth.get());


	// -- lives display boss --
	auto displayBossHealth = std::make_unique<dae::GameObject>();
	// Transform
	displayBossHealth->AddComponent<TransformComponent>(displayBossHealth.get());
	displayBossHealth->GetComponent<TransformComponent>()->SetWorldPosition(100, 300);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayBossHealth->AddComponent<TextComponent>(displayBossHealth.get(), std::move(smallFont));
	// Render
	displayBossHealth->AddComponent<RenderComponent>(displayBossHealth.get());
	// LivesDisplay
	displayBossHealth->AddComponent<dae::DisplayComponentHealth>(displayBossHealth.get());


	// -- score display fighter --
	auto displayFighterScore = std::make_unique<dae::GameObject>();
	// Transform
	displayFighterScore->AddComponent<TransformComponent>(displayFighterScore.get());
	displayFighterScore->GetComponent<TransformComponent>()->SetWorldPosition(100, 270);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayFighterScore->AddComponent<TextComponent>(displayFighterScore.get(), std::move(smallFont));
	// Render
	displayFighterScore->AddComponent<RenderComponent>(displayFighterScore.get());
	// LivesDisplay
	displayFighterScore->AddComponent<dae::DisplayComponentScore>(displayFighterScore.get());


	// -- score display boss --
	auto displayBossScore = std::make_unique<dae::GameObject>();
	// Transform
	displayBossScore->AddComponent<TransformComponent>(displayBossScore.get());
	displayBossScore->GetComponent<TransformComponent>()->SetWorldPosition(100, 320);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayBossScore->AddComponent<TextComponent>(displayBossScore.get(), std::move(smallFont));
	// Render
	displayBossScore->AddComponent<RenderComponent>(displayBossScore.get());
	// LivesDisplay
	displayBossScore->AddComponent<dae::DisplayComponentScore>(displayBossScore.get());


	fighter->GetComponent<dae::HealthComponent>()->AddObserver(displayFighterHealth->GetComponent<dae::DisplayComponentHealth>());
	fighter->GetComponent<dae::ScoreComponent>()->AddObserver(displayFighterScore->GetComponent<dae::DisplayComponentScore>());

	boss->GetComponent<dae::HealthComponent>()->AddObserver(displayBossHealth->GetComponent<dae::DisplayComponentHealth>());
	boss->GetComponent<dae::ScoreComponent>()->AddObserver(displayBossScore->GetComponent<dae::DisplayComponentScore>());

	// Input
	// add controllers
	dae::InputManager::GetInstance().AddController();

	// fighter
	float speed{ 50 };
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_LEFT), std::make_unique<dae::MoveCommand>(fighter.get(), glm::vec2{ -speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_RIGHT), std::make_unique<dae::MoveCommand>(fighter.get(), glm::vec2{ speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_UP), std::make_unique<dae::MoveCommand>(fighter.get(), glm::vec2{ 0, -speed }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_DOWN), std::make_unique<dae::MoveCommand>(fighter.get(), glm::vec2{ 0, speed }), dae::InputActionType::Continuous);

	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_A), std::make_unique<dae::DamageCommand>(fighter.get(), 1.f), dae::InputActionType::OnPressed);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_X), std::make_unique<dae::ScoreGainCommand>(fighter.get(), 100), dae::InputActionType::OnPressed);

	// boss
	speed = 100;
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_a), std::make_unique<dae::MoveCommand>(boss.get(), glm::vec2{ -speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_d), std::make_unique<dae::MoveCommand>(boss.get(), glm::vec2{ speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_w), std::make_unique<dae::MoveCommand>(boss.get(), glm::vec2{ 0, -speed }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_s), std::make_unique<dae::MoveCommand>(boss.get(), glm::vec2{ 0, speed }), dae::InputActionType::Continuous);

	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_f), std::make_unique<dae::DamageCommand>(boss.get(), 1.f), dae::InputActionType::OnPressed);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_x), std::make_unique<dae::ScoreGainCommand>(boss.get(), 100), dae::InputActionType::OnPressed);

	// add gameObjects to scene
	scene.Add(std::move(backGround));
	scene.Add(std::move(logo));
	scene.Add(std::move(textA));
	scene.Add(std::move(textDPad));
	scene.Add(std::move(textWASD));
	scene.Add(std::move(fpsCounter));
	scene.Add(std::move(fighter));
	scene.Add(std::move(boss));
	scene.Add(std::move(displayFighterHealth));
	scene.Add(std::move(displayBossHealth));
	scene.Add(std::move(displayFighterScore));
	scene.Add(std::move(displayBossScore));
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}