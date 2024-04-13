#pragma once
#include "GameObject.h"

#include <string>

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		void DestroyObjects();

		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter; 
	};
}
