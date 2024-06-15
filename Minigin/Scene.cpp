#include "Scene.h"
#include "TextComponent.h"
#include <algorithm>
#include <vector>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_NewObjects.emplace_back(std::move(object));
}

void Scene::FixedUpdate()
{
	for (const auto& object : m_Objects)
	{
		if (object && object->GetActive())
			object->FixedUpdate();
	}
}

void Scene::Update()
{
	for (const auto& object : m_Objects)
	{
		if (object && object->GetActive())
			object->Update();
	}
}

void Scene::LateUpdate()
{
	for (const auto& object : m_Objects)
	{
		if (object && object->GetActive())
			object->LateUpdate();
	}

	// Destroy game objects if needed
	DestroyObjects();

	// Add new objects to the main vector
	for (auto& object : m_NewObjects)
	{
		m_Objects.emplace_back(std::move(object));
	}
	m_NewObjects.clear();
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if(object && object->GetActive())
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

const std::vector<GameObject*> Scene::GetAllGameObjects() const
{
	std::vector<GameObject*> objects{};
	objects.reserve(m_Objects.size());
	for (const auto& object : m_Objects)
	{
		objects.emplace_back(object.get());
	}
	return objects;
}

const std::vector<GameObject*> Scene::GetGameObjectsWithTag(const std::string& tag) const
{
	std::vector<GameObject*> taggedObjects{};
	for (const auto& object : m_Objects)
	{
		if (object->GetTag() == tag)
		{
			taggedObjects.push_back(object.get());
		}
	}
	return taggedObjects;
}
