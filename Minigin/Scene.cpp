#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include "TextComponent.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		if (object->GetActive())
			object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		if (object->GetActive())
			object->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		if (object->GetActive())
			object->LateUpdate();
	}

	// Destroy game objects if needed
	DestroyObjects();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Scene::DestroyObjects()
{
	for (const auto& object : m_objects)
	{
		object->DestroyComponents();
	}

	std::erase_if(m_objects,
		[&](const std::shared_ptr<GameObject>& object)
		{
			return object->IsMarkedForDestroy();
		}
	);
}

