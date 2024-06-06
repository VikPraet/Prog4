#include "Scenes.h"

#include <Windows.h>
#include <Xinput.h>

#include "BasicEnemyMovementBehavior.h"
#include "ColliderComponent.h"
#include "ColliderRenderComponent.h"
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
#include "GameCommands.h"
#include "ParticleRenderComponent.h"
#include "ParticleSystemComponent.h"
#include "PlayerMovementBehavior.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "WaveManager.h"

void galaga::LoadMainScene()
{
	auto& scene = dae::SceneManager::GetInstance().LoadScene("MainScene");

	// Initialize the WaveManager
	WaveManager waveManager;
	// Load waves from the file
	waveManager.LoadWavesFromFile("../Data/waves.txt");

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
	redParticleSystem->SetSeedSize(2.5f, true, 1.5f);
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
	greenParticleSystem->SetSeedSize(2.5f, true, 1.5f);
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
	blueParticleSystem->SetSeedSize(2.5f, true, 1.5f);
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

	// -- player --
	auto player = std::make_unique<dae::GameObject>();
	// Transform
	player->AddComponent<dae::TransformComponent>(player.get());
	player->GetComponent<dae::TransformComponent>()->SetWorldPosition(static_cast<float>(dae::Settings::window_width / 2), static_cast<float>(dae::Settings::window_height - 80));
	player->GetComponent<dae::TransformComponent>()->SetScale(2);
	// Render
	player->AddComponent<dae::RenderComponent>(player.get());
	player->GetComponent<dae::RenderComponent>()->SetTexture("galaga-fighter.png");
	// Movement
	player->AddComponent<PlayerMovementBehavior>(player.get());
	player->GetComponent<PlayerMovementBehavior>()->SetSpeed(150.f);
	// Attack
	player->AddComponent<PlayerAttackBehavior>(player.get());
	player->GetComponent<PlayerAttackBehavior>()->SetFireRate(0.3f);
	// Collider
	player->AddComponent<dae::ColliderComponent>(player.get(), glm::vec2(33.f, 33.f));
	//player->AddComponent<dae::ColliderRenderComponent>(player.get());

	// -- Input --
	// Create controller
	dae::InputManager::GetInstance().AddController();

	// Movement
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_LEFT), std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_DPAD_RIGHT), std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_a), std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_d), std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_LEFT), std::make_unique<MoveCommand>(player.get(), glm::vec2{ -1, 0 }), dae::InputActionType::Continuous);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_RIGHT), std::make_unique<MoveCommand>(player.get(), glm::vec2{ 1, 0 }), dae::InputActionType::Continuous);

	dae::InputManager::GetInstance().BindThumbCommand(dae::GamepadStick(XINPUT_GAMEPAD_LEFT_THUMB), std::make_unique<ThumbMoveCommand>(player.get()), dae::InputActionType::Continuous);

	// Attack
	dae::InputManager::GetInstance().BindCommand(dae::GamepadButton(XINPUT_GAMEPAD_A), std::make_unique<ShootCommand>(player.get()), dae::InputActionType::OnPressed);
	dae::InputManager::GetInstance().BindCommand(dae::KeyboardKey(SDLK_SPACE), std::make_unique<ShootCommand>(player.get()), dae::InputActionType::OnPressed);

	// Add GameObjects to the scene
	scene.Add(std::move(redParticles));
	scene.Add(std::move(greenParticles));
	scene.Add(std::move(blueParticles));
	scene.Add(std::move(fpsCounter));
	scene.Add(std::move(player));

	// Spawn the first wave
	waveManager.SpawnWave(0);

	ServiceLocator::GetService<ISoundService>()->SetSoundVolume(15);
	ServiceLocator::GetService<ISoundService>()->SetMusicVolume(25);
	ServiceLocator::GetService<ISoundService>()->PlayMusic("../Data/GalagaTheme.wav");
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

	// Add GameObjects to the scene
	scene.Add(std::move(redParticles));
	scene.Add(std::move(greenParticles));
	scene.Add(std::move(blueParticles));
	scene.Add(std::move(fpsCounter));

	ServiceLocator::GetService<ISoundService>()->SetSoundVolume(15);
	ServiceLocator::GetService<ISoundService>()->SetMusicVolume(25);
	ServiceLocator::GetService<ISoundService>()->PlaySoundEffect("../Data/1-Up.wav");
	ServiceLocator::GetService<ISoundService>()->PlayMusic("../Data/GalagaTheme.wav");
}
