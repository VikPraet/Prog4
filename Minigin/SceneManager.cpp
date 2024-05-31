#include "SceneManager.h"
#include <ranges>
#include <stdexcept>

void dae::SceneManager::FixedUpdate()
{
	if (m_ActiveScene)
	{
		m_ActiveScene->FixedUpdate();
	}
}

void dae::SceneManager::Update()
{
	if (m_ActiveScene)
	{
		m_ActiveScene->Update();
	}
}

void dae::SceneManager::LateUpdate()
{
	if (m_ActiveScene)
	{
		m_ActiveScene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	if (m_ActiveScene)
	{
		m_ActiveScene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	auto& ref = *scene;
	m_Scenes[name] = std::move(scene);
	return ref;
}

dae::Scene& dae::SceneManager::LoadScene(const std::string& name)
{
	const auto scene = m_Scenes.find(name);
	if (scene != m_Scenes.end())
	{
		m_ActiveScene = scene->second.get();
		return *scene->second;
	}
	Scene& newScene = CreateScene(name);
	m_ActiveScene = &newScene;
	return newScene;
}

void dae::SceneManager::UnLoadScene(const std::string& name)
{
	const auto scene = m_Scenes.find(name);
	if (scene != m_Scenes.end())
	{
		if (m_ActiveScene == scene->second.get())
		{
			m_ActiveScene = nullptr;
		}
		m_Scenes.erase(scene);
	}
}

dae::Scene* dae::SceneManager::GetActiveScene() const
{
	return m_ActiveScene;
}
