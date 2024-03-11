#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <windows.h>

#include "Settings.h"
#include "Minigin.h"
#include "FpsComponent.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "RotationComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "TTCComponent.h"

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

	// -- text --
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto text = std::make_shared<dae::GameObject>();
	// Transform
	text->AddComponent<TransformComponent>(text);
	text->GetComponent<TransformComponent>()->SetWorldPosition(Settings::window_width / 2, 50);
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
	fpsCounter->GetComponent<TransformComponent>()->SetWorldPosition(100, 450);
	// Fps
	fpsCounter->AddComponent<FpsComponent>(fpsCounter);
	// Text
	fpsCounter->AddComponent<TextComponent>(fpsCounter, font);
	// Render
	fpsCounter->AddComponent<RenderComponent>(fpsCounter);
	// Add to scene
	scene.Add(fpsCounter);

	// -- FPS counter --
	auto TTCUI = std::make_shared<dae::GameObject>();
	// Transform
	TTCUI->AddComponent<TTCComponent>(TTCUI);
	// Add to scene
	scene.Add(TTCUI);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}