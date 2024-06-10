#pragma once
#include "GameObject.h"

#include <string>

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		void DestroyObjects();
		const std::vector<GameObject*> GetAllGameObjects() const;
		const std::vector<GameObject*> GetGameObjectsWithTag(const std::string& tag) const;

		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects{};
		std::vector<std::unique_ptr<GameObject>> m_NewObjects{};

		static unsigned int m_IdCounter; 
	};
}
