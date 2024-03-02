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
	auto go = std::make_shared<dae::GameObject>();
	// Transform
	go->AddComponent<TransformComponent>(go);
	// Render
	go->AddComponent<RenderComponent>(go);
	go->GetComponent<RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	// -- logo --
	go = std::make_shared<dae::GameObject>();
	// Transform
	go->AddComponent<TransformComponent>(go);
	go->GetComponent<TransformComponent>()->SetPosition(216, 180);
	// Render
	go->AddComponent<RenderComponent>(go);
	go->GetComponent<RenderComponent>()->SetTexture("logo.tga");
	scene.Add(go);

	// -- text --
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>();
	// Transform
	go->AddComponent<TransformComponent>(go);
	go->GetComponent<TransformComponent>()->SetPosition(80, 20);
	// Text
	go->AddComponent<TextComponent>(go, "Programming 4 Assignment", font);
	// Render
	go->AddComponent<RenderComponent>(go);
	scene.Add(go);

	// -- FPS counter --
	go = std::make_shared<dae::GameObject>();
	// Transform
	go->AddComponent<TransformComponent>(go);
	go->GetComponent<TransformComponent>()->SetPosition(20, 430);
	// Fps
	go->AddComponent<FpsComponent>(go);
	// Text
	go->AddComponent<TextComponent>(go, font);
	// Render
	go->AddComponent<RenderComponent>(go);
	scene.Add(go);

	// -- Rotating ship --
	go = std::make_shared<dae::GameObject>();
	// Transform
	go->AddComponent<TransformComponent>(go);
	go->GetComponent<TransformComponent>()->SetPosition(300, 200);
	go->GetComponent<TransformComponent>()->SetScale(0.04f, 0.04f);
	// Render
	go->AddComponent<RenderComponent>(go);
	go->GetComponent<RenderComponent>()->SetTexture("galaga-Ship.png");
	// Rotation
	go->AddComponent<RotationComponent>(go, 100.f, 0.7f);
	go->GetComponent<RotationComponent>()->SetRotateImage(true);
	scene.Add(go);

	// -- Rotating ship --
	go = std::make_shared<dae::GameObject>();
	// Transform
	go->AddComponent<TransformComponent>(go);
	go->GetComponent<TransformComponent>()->SetPosition(470, 300);
	go->GetComponent<TransformComponent>()->SetScale(0.04f, 0.04f);
	// Render
	go->AddComponent<RenderComponent>(go);
	go->GetComponent<RenderComponent>()->SetTexture("galaga-boss.png");
	// Rotation
	go->AddComponent<RotationComponent>(go, 50.f, -1.f);
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}