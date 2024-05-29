#include "SceneManager.h"

#include <ranges>

void dae::SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes | std::views::values)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::Update()
{
	for (auto& scene : m_Scenes | std::views::values)
	{
		scene->Update();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_Scenes | std::views::values)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& pair : m_Scenes)
	{
		pair.second->Render();
	}
}

dae::Scene& dae::SceneManager::LoadScene(const std::string& name)
{
	m_Scenes.clear();

	auto scene = std::make_unique<Scene>(name);
	auto& ref = *scene;
	m_Scenes[name] = std::move(scene);
	return ref;
}

void dae::SceneManager::UnLoadScene(const std::string& name)
{
	const auto scene = m_Scenes.find(name);
	if (scene != m_Scenes.end())
	{
		m_Scenes.erase(scene);
	}
}
