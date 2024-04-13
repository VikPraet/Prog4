#include "Scene.h"
#include "TextComponent.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_Objects.emplace_back(std::move(object));
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		if (object->GetActive())
			object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		if (object->GetActive())
			object->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_Objects)
	{
		if (object->GetActive())
			object->LateUpdate();
	}

	// Destroy game objects if needed
	DestroyObjects();
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void Scene::DestroyObjects()
{
	for (const auto& object : m_Objects)
	{
		object->DestroyComponents();
	}

	std::erase_if(m_Objects,
		[&](const std::unique_ptr<GameObject>& object)
		{
			return object->IsMarkedForDestroy();
		}
	);
}

