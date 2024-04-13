#include "SceneManager.h"

void dae::SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	m_scenes.push_back(std::move(scene));
	return *m_scenes[m_scenes.size() - 1];
}