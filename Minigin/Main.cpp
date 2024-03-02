#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FpsComponent.h"
#include "Minigin.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "RotationComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// -- background --
	auto backGround = std::make_shared<dae::GameObject>();
	// Transform
	backGround->AddComponent<TransformComponent>(backGround);
	// Render
	backGround->AddComponent<RenderComponent>(backGround);
	backGround->GetComponent<RenderComponent>()->SetTexture("background.tga");
	// Add to scene
	scene.Add(backGround);

	// -- logo --
	auto logo = std::make_shared<dae::GameObject>();
	// Transform
	logo->AddComponent<TransformComponent>(logo);
	logo->GetComponent<TransformComponent>()->SetWorldPosition(216, 180);
	// Render
	logo->AddComponent<RenderComponent>(logo);
	logo->GetComponent<RenderComponent>()->SetTexture("logo.tga");
	// Add to scene
	scene.Add(logo);

	// -- text --
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto text = std::make_shared<dae::GameObject>();
	// Transform
	text->AddComponent<TransformComponent>(text);
	text->GetComponent<TransformComponent>()->SetWorldPosition(80, 20);
	// Text
	text->AddComponent<TextComponent>(text, "Programming 4 Assignment", font);
	// Render
	text->AddComponent<RenderComponent>(text);
	// Add to scene
	scene.Add(text);

	// -- FPS counter --
	auto fpsCounter = std::make_shared<dae::GameObject>();
	// Transform
	fpsCounter->AddComponent<TransformComponent>(fpsCounter);
	fpsCounter->GetComponent<TransformComponent>()->SetWorldPosition(20, 430);
	// Fps
	fpsCounter->AddComponent<FpsComponent>(fpsCounter);
	// Text
	fpsCounter->AddComponent<TextComponent>(fpsCounter, font);
	// Render
	fpsCounter->AddComponent<RenderComponent>(fpsCounter);
	// Add to scene
	scene.Add(fpsCounter);

	// -- Fighter --
	auto ship = std::make_shared<dae::GameObject>();
	// Transform
	ship->AddComponent<TransformComponent>(ship);
	ship->GetComponent<TransformComponent>()->SetWorldPosition(430, 300);
	ship->GetComponent<TransformComponent>()->SetScale(2.f);
	// Render
	ship->AddComponent<RenderComponent>(ship);
	ship->GetComponent<RenderComponent>()->SetTexture("galaga-fighter.png");
	// Rotation
	ship->AddComponent<RotationComponent>(ship, 50.f, -1.5f);
	// Add to scene
	scene.Add(ship);

	// -- Boss --
	auto boss = std::make_shared<dae::GameObject>();
	// Transform
	boss->AddComponent<TransformComponent>(boss);
	boss->GetComponent<TransformComponent>()->SetScale(2.f);
	// Render
	boss->AddComponent<RenderComponent>(boss);
	boss->GetComponent<RenderComponent>()->SetTexture("galaga-boss.png");
	// Rotation
	boss->AddComponent<RotationComponent>(boss, 70.f, 2.f);
	// Set parent
	boss->SetParent(ship.get());
	// Add to scene
	scene.Add(boss);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}