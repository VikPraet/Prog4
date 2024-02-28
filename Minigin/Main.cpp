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
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}