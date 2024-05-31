#include "Scenes.h"

#include <Windows.h>
#include <Xinput.h>

#include "SceneManager.h"
#include "Settings.h"
#include "FpsComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "DisplayComponentHealth.h"
#include "DisplayComponentScore.h"
#include "commands.h"
#include "ParticleRenderComponent.h"
#include "ParticleSystemComponent.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

void galaga::LoadMainScene()
{
	auto& scene = dae::SceneManager::GetInstance().LoadScene("MainScene");

	// -- background --
	auto backGround = std::make_unique<dae::GameObject>();
	// Transform
	backGround->AddComponent<dae::TransformComponent>(backGround.get());
	backGround->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), static_cast<float>(dae::Settings::window_height / 2));
	// Render
	backGround->AddComponent<dae::RenderComponent>(backGround.get());
	backGround->GetComponent<dae::RenderComponent>()->SetTexture("background.tga");


	// -- logo --
	auto logo = std::make_unique<dae::GameObject>();
	// Transform
	logo->AddComponent<dae::TransformComponent>(logo.get());
	logo->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), 230);
	// Render
	logo->AddComponent<dae::RenderComponent>(logo.get());
	logo->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");


	// -- text (assignment) --
	auto textA = std::make_unique<dae::GameObject>();
	// Transform
	textA->AddComponent<dae::TransformComponent>(textA.get());
	textA->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), 50);
	// Text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textA->AddComponent<dae::TextComponent>(textA.get(), "Programming 4 Assignment", std::move(font));
	// Render
	textA->AddComponent<dae::RenderComponent>(textA.get());


	// -- text (D-pad) --
	auto textDPad = std::make_unique<dae::GameObject>();
	// Transform
	textDPad->AddComponent<dae::TransformComponent>(textDPad.get());
	textDPad->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), 130);
	// Text
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	textDPad->AddComponent<dae::TextComponent>(textDPad.get(), "use the D-Pad to move the fighter, A to damage, X to increase score", std::move(smallFont));
	// Render
	textDPad->AddComponent<dae::RenderComponent>(textDPad.get());


	// -- text (WASD) --
	auto textWASD = std::make_unique<dae::GameObject>();
	// Transform
	textWASD->AddComponent<dae::TransformComponent>(textWASD.get());
	textWASD->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), 160);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	textWASD->AddComponent<dae::TextComponent>(textWASD.get(), "use WASD to move the boss, F to damage, X to increase score", std::move(smallFont));
	// Render
	textWASD->AddComponent<dae::RenderComponent>(textWASD.get());


	// -- FPS counter --
	auto fpsCounter = std::make_unique<dae::GameObject>();
	// Transform
	fpsCounter->AddComponent<dae::TransformComponent>(fpsCounter.get());
	fpsCounter->GetComponent<dae::TransformComponent>()->SetWorldPosition(50, 20);
	// Fps
	fpsCounter->AddComponent<dae::FpsComponent>(fpsCounter.get());
	// Text
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	fpsCounter->AddComponent<dae::TextComponent>(fpsCounter.get(), std::move(font));
	// Render
	fpsCounter->AddComponent<dae::RenderComponent>(fpsCounter.get());


	// -- Fighter --
	auto fighter = std::make_unique<dae::GameObject>();
	// Transform
	fighter->AddComponent<dae::TransformComponent>(fighter.get());
	fighter->GetComponent<dae::TransformComponent>()->SetWorldPosition(200, 300);
	fighter->GetComponent<dae::TransformComponent>()->SetScale(2);
	// Render
	fighter->AddComponent<dae::RenderComponent>(fighter.get());
	fighter->GetComponent<dae::RenderComponent>()->SetTexture("galaga-fighter.png");
	// Health
	fighter->AddComponent<HealthComponent>(fighter.get(), 3.f);
	// Score
	fighter->AddComponent<ScoreComponent>(fighter.get());


	// -- Boss --
	auto boss = std::make_unique<dae::GameObject>();
	// Transform
	boss->AddComponent<dae::TransformComponent>(boss.get());
	boss->GetComponent<dae::TransformComponent>()->SetWorldPosition(250, 300);
	boss->GetComponent<dae::TransformComponent>()->SetScale(2);
	// Render
	boss->AddComponent<dae::RenderComponent>(boss.get());
	boss->GetComponent<dae::RenderComponent>()->SetTexture("galaga-boss.png");
	// Health
	boss->AddComponent<HealthComponent>(boss.get(), 3.f);
	// Score
	boss->AddComponent<ScoreComponent>(boss.get());


	// -- lives display fighter --
	auto displayFighterHealth = std::make_unique<dae::GameObject>();
	// Transform
	displayFighterHealth->AddComponent<dae::TransformComponent>(displayFighterHealth.get());
	displayFighterHealth->GetComponent<dae::TransformComponent>()->SetWorldPosition(100, 250);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayFighterHealth->AddComponent<dae::TextComponent>(displayFighterHealth.get(), std::move(smallFont));
	// Render
	displayFighterHealth->AddComponent<dae::RenderComponent>(displayFighterHealth.get());
	// LivesDisplay
	displayFighterHealth->AddComponent<DisplayComponentHealth>(displayFighterHealth.get(), fighter->GetComponent<HealthComponent>());


	// -- lives display boss --
	auto displayBossHealth = std::make_unique<dae::GameObject>();
	// Transform
	displayBossHealth->AddComponent<dae::TransformComponent>(displayBossHealth.get());
	displayBossHealth->GetComponent<dae::TransformComponent>()->SetWorldPosition(100, 300);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayBossHealth->AddComponent<dae::TextComponent>(displayBossHealth.get(), std::move(smallFont));
	// Render
	displayBossHealth->AddComponent<dae::RenderComponent>(displayBossHealth.get());
	// LivesDisplay
	displayBossHealth->AddComponent<DisplayComponentHealth>(displayBossHealth.get(), boss->GetComponent<HealthComponent>());


	// -- score display fighter --
	auto displayFighterScore = std::make_unique<dae::GameObject>();
	// Transform
	displayFighterScore->AddComponent<dae::TransformComponent>(displayFighterScore.get());
	displayFighterScore->GetComponent<dae::TransformComponent>()->SetWorldPosition(100, 270);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayFighterScore->AddComponent<dae::TextComponent>(displayFighterScore.get(), std::move(smallFont));
	// Render
	displayFighterScore->AddComponent<dae::RenderComponent>(displayFighterScore.get());
	// LivesDisplay
	displayFighterScore->AddComponent<DisplayComponentScore>(displayFighterScore.get(), fighter->GetComponent<ScoreComponent>());


	// -- score display boss --
	auto displayBossScore = std::make_unique<dae::GameObject>();
	// Transform
	displayBossScore->AddComponent<dae::TransformComponent>(displayBossScore.get());
	displayBossScore->GetComponent<dae::TransformComponent>()->SetWorldPosition(100, 320);
	// Text
	smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	displayBossScore->AddComponent<dae::TextComponent>(displayBossScore.get(), std::move(smallFont));
	// Render
	displayBossScore->AddComponent<dae::RenderComponent>(displayBossScore.get());
	// LivesDisplay
	displayBossScore->AddComponent<DisplayComponentScore>(displayBossScore.get(), boss->GetComponent<ScoreComponent>());

	// Input
	// add controllers
	dae::InputManager::GetInstance().AddController();

	// fighter
	float speed{ 50 };
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_LEFT), std::make_unique<galaga::MoveCommand>(fighter.get(), glm::vec2{ -speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_RIGHT), std::make_unique<galaga::MoveCommand>(fighter.get(), glm::vec2{ speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_UP), std::make_unique<galaga::MoveCommand>(fighter.get(), glm::vec2{ 0, -speed }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_DOWN), std::make_unique<galaga::MoveCommand>(fighter.get(), glm::vec2{ 0, speed }), dae::InputActionType::Continuous);

	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_A), std::make_unique<galaga::DamageCommand>(fighter.get(), 1.f), dae::InputActionType::OnPressed);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_X), std::make_unique<galaga::ScoreGainCommand>(fighter.get(), 100), dae::InputActionType::OnPressed);

	// boss
	speed = 100;
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_a), std::make_unique<galaga::MoveCommand>(boss.get(), glm::vec2{ -speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_d), std::make_unique<galaga::MoveCommand>(boss.get(), glm::vec2{ speed, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_w), std::make_unique<galaga::MoveCommand>(boss.get(), glm::vec2{ 0, -speed }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_s), std::make_unique<galaga::MoveCommand>(boss.get(), glm::vec2{ 0, speed }), dae::InputActionType::Continuous);

	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_f), std::make_unique<galaga::DamageCommand>(boss.get(), 1.f), dae::InputActionType::OnPressed);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_x), std::make_unique<galaga::ScoreGainCommand>(boss.get(), 100), dae::InputActionType::OnPressed);

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

void galaga::LoadMainMenuScene()
{
	auto& scene = dae::SceneManager::GetInstance().LoadScene("MainMenuScene");

	// -- red particles --
	auto redParticles = std::make_unique<dae::GameObject>();
	// Particle system
	redParticles->AddComponent<dae::ParticleSystemComponent>(redParticles.get());
	const auto redParticleSystem = redParticles->GetComponent<dae::ParticleSystemComponent>();
	// Particle render
	redParticles->AddComponent<dae::ParticleRenderComponent>(redParticles.get(), redParticleSystem);
	// Set seed properties for red particles
	redParticleSystem->SetSeedColor(255, 100, 100, 255, true, 0, 155, 155, 0);
	redParticleSystem->SetSeedVelocity(0.0f, 130.0f, true, 0.0f, 60.0f);
	redParticleSystem->SetSeedLifespan(7.f, true, 3.f);
	redParticleSystem->SetSeedSize(3.5f, true, 2.0f);
	redParticleSystem->SetSpawnArea(static_cast<float>(dae::Settings::window_width / 2), -5.0f, true, static_cast<float>(dae::Settings::window_width), 5.0f);
	// Set emission properties for red particles
	redParticleSystem->SetTargetNumberOfParticles(25);
	redParticleSystem->SetEmissionRate(2.0f);
	redParticleSystem->SetEmissionMode(dae::ParticleSystemComponent::EmissionMode::Continuous);

	// -- green particles --
	auto greenParticles = std::make_unique<dae::GameObject>();
	// Particle system
	greenParticles->AddComponent<dae::ParticleSystemComponent>(greenParticles.get());
	const auto greenParticleSystem = greenParticles->GetComponent<dae::ParticleSystemComponent>();
	// Particle render
	greenParticles->AddComponent<dae::ParticleRenderComponent>(greenParticles.get(), greenParticleSystem);
	// Set seed properties for green particles
	greenParticleSystem->SetSeedColor(100, 255, 100, 255, true, 155, 0, 155, 0);
	greenParticleSystem->SetSeedVelocity(0.0f, 130.0f, true, 0.0f, 60.0f);
	greenParticleSystem->SetSeedLifespan(7.f, true, 3.f);
	greenParticleSystem->SetSeedSize(3.5f, true, 2.0f);
	greenParticleSystem->SetSpawnArea(static_cast<float>(dae::Settings::window_width / 2), -5.0f, true, static_cast<float>(dae::Settings::window_width), 5.0f);
	// Set emission properties for green particles
	greenParticleSystem->SetTargetNumberOfParticles(25);
	greenParticleSystem->SetEmissionRate(2.0f);
	greenParticleSystem->SetEmissionMode(dae::ParticleSystemComponent::EmissionMode::Continuous);

	// -- blue particles --
	auto blueParticles = std::make_unique<dae::GameObject>();
	// Particle system
	blueParticles->AddComponent<dae::ParticleSystemComponent>(blueParticles.get());
	const auto blueParticleSystem = blueParticles->GetComponent<dae::ParticleSystemComponent>();
	// Particle render
	blueParticles->AddComponent<dae::ParticleRenderComponent>(blueParticles.get(), blueParticleSystem);
	// Set seed properties for blue particles
	blueParticleSystem->SetSeedColor(100, 100, 255, 255, true, 155, 155, 0, 0);
	blueParticleSystem->SetSeedVelocity(0.0f, 130.0f, true, 0.0f, 60.0f);
	blueParticleSystem->SetSeedLifespan(7.f, true, 3.f);
	blueParticleSystem->SetSeedSize(3.5f, true, 2.0f);
	blueParticleSystem->SetSpawnArea(static_cast<float>(dae::Settings::window_width / 2), -5.0f, true, static_cast<float>(dae::Settings::window_width), 5.0f);
	// Set emission properties for blue particles
	blueParticleSystem->SetTargetNumberOfParticles(25);
	blueParticleSystem->SetEmissionRate(2.0f);
	blueParticleSystem->SetEmissionMode(dae::ParticleSystemComponent::EmissionMode::Continuous);

	scene.Add(std::move(redParticles));
	scene.Add(std::move(greenParticles));
	scene.Add(std::move(blueParticles));
}

void galaga::LoadTestScene()
{
	auto& scene = dae::SceneManager::GetInstance().LoadScene("TestScene");

		// -- red particles --
	auto redParticles = std::make_unique<dae::GameObject>();
	// Particle system
	redParticles->AddComponent<dae::ParticleSystemComponent>(redParticles.get());
	const auto redParticleSystem = redParticles->GetComponent<dae::ParticleSystemComponent>();
	// Particle render
	redParticles->AddComponent<dae::ParticleRenderComponent>(redParticles.get(), redParticleSystem);
	// Set seed properties for red particles
	redParticleSystem->SetSeedColor(255, 100, 100, 255, true, 0, 155, 155, 0);
	redParticleSystem->SetSeedVelocity(0.0f, 130.0f, true, 0.0f, 60.0f);
	redParticleSystem->SetSeedLifespan(7.f, true, 3.f);
	redParticleSystem->SetSeedSize(3.5f, true, 2.0f);
	redParticleSystem->SetSpawnArea(static_cast<float>(dae::Settings::window_width / 2), -5.0f, true, static_cast<float>(dae::Settings::window_width), 5.0f);
	// Set emission properties for red particles
	redParticleSystem->SetTargetNumberOfParticles(25);
	redParticleSystem->SetEmissionRate(2.0f);
	redParticleSystem->SetEmissionMode(dae::ParticleSystemComponent::EmissionMode::Continuous);

	// -- green particles --
	auto greenParticles = std::make_unique<dae::GameObject>();
	// Particle system
	greenParticles->AddComponent<dae::ParticleSystemComponent>(greenParticles.get());
	const auto greenParticleSystem = greenParticles->GetComponent<dae::ParticleSystemComponent>();
	// Particle render
	greenParticles->AddComponent<dae::ParticleRenderComponent>(greenParticles.get(), greenParticleSystem);
	// Set seed properties for green particles
	greenParticleSystem->SetSeedColor(100, 255, 100, 255, true, 155, 0, 155, 0);
	greenParticleSystem->SetSeedVelocity(0.0f, 130.0f, true, 0.0f, 60.0f);
	greenParticleSystem->SetSeedLifespan(7.f, true, 3.f);
	greenParticleSystem->SetSeedSize(3.5f, true, 2.0f);
	greenParticleSystem->SetSpawnArea(static_cast<float>(dae::Settings::window_width / 2), -5.0f, true, static_cast<float>(dae::Settings::window_width), 5.0f);
	// Set emission properties for green particles
	greenParticleSystem->SetTargetNumberOfParticles(25);
	greenParticleSystem->SetEmissionRate(2.0f);
	greenParticleSystem->SetEmissionMode(dae::ParticleSystemComponent::EmissionMode::Continuous);

	// -- blue particles --
	auto blueParticles = std::make_unique<dae::GameObject>();
	// Particle system
	blueParticles->AddComponent<dae::ParticleSystemComponent>(blueParticles.get());
	const auto blueParticleSystem = blueParticles->GetComponent<dae::ParticleSystemComponent>();
	// Particle render
	blueParticles->AddComponent<dae::ParticleRenderComponent>(blueParticles.get(), blueParticleSystem);
	// Set seed properties for blue particles
	blueParticleSystem->SetSeedColor(100, 100, 255, 255, true, 155, 155, 0, 0);
	blueParticleSystem->SetSeedVelocity(0.0f, 130.0f, true, 0.0f, 60.0f);
	blueParticleSystem->SetSeedLifespan(7.f, true, 3.f);
	blueParticleSystem->SetSeedSize(3.5f, true, 2.0f);
	blueParticleSystem->SetSpawnArea(static_cast<float>(dae::Settings::window_width / 2), -5.0f, true, static_cast<float>(dae::Settings::window_width), 5.0f);
	// Set emission properties for blue particles
	blueParticleSystem->SetTargetNumberOfParticles(25);
	blueParticleSystem->SetEmissionRate(2.0f);
	blueParticleSystem->SetEmissionMode(dae::ParticleSystemComponent::EmissionMode::Continuous);

	// -- FPS counter --
	auto fpsCounter = std::make_unique<dae::GameObject>();
	// Transform
	fpsCounter->AddComponent<dae::TransformComponent>(fpsCounter.get());
	fpsCounter->GetComponent<dae::TransformComponent>()->SetWorldPosition(50, 20);
	// Fps
	fpsCounter->AddComponent<dae::FpsComponent>(fpsCounter.get());
	// Text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	fpsCounter->AddComponent<dae::TextComponent>(fpsCounter.get(), std::move(font));
	// Render
	fpsCounter->AddComponent<dae::RenderComponent>(fpsCounter.get());

	scene.Add(std::move(redParticles));
	scene.Add(std::move(greenParticles));
	scene.Add(std::move(blueParticles));
	scene.Add(std::move(fpsCounter));

	ServiceLocator::GetService<ISoundService>()->SetSoundVolume(15);
	ServiceLocator::GetService<ISoundService>()->SetMusicVolume(25);
	ServiceLocator::GetService<ISoundService>()->PlaySoundEffect("../Data/1-Up.wav");
	ServiceLocator::GetService<ISoundService>()->PlayMusic("../Data/GalagaTheme.wav");
}
