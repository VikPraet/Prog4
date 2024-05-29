#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& LoadScene(const std::string& name);
		void UnLoadScene(const std::string& name);

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::unordered_map<std::string, std::unique_ptr<Scene>> m_Scenes;
	};
}
