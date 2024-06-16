#include "LivesDisplay.h"

#include "LivesManager.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "Settings.h"
#include "TransformComponent.h"

galaga::LivesDisplay::LivesDisplay(dae::GameObject* gameObject)
	:BaseComponent(gameObject)
{
	LivesManager::GetInstance().OnLivesChanged.AddListener(this, &LivesDisplay::OnLivesChanged);
	OnLivesChanged(LivesManager::GetInstance().GetLives());
}

void galaga::LivesDisplay::OnLivesChanged(int lives)
{
	GetGameObject()->DestroyChildren();

	for(int i{}; i < lives; ++i)
	{
		InstantiateIcon(static_cast<float>(30 * i + 16));
	}
}

void galaga::LivesDisplay::InstantiateIcon(float xPos)
{
	auto gameObject = std::make_unique<dae::GameObject>();

	gameObject->AddComponent<dae::TransformComponent>(gameObject.get());
	gameObject->GetComponent<dae::TransformComponent>()->SetScale(2.f);
	gameObject->GetComponent<dae::TransformComponent>()->SetWorldPosition(xPos, static_cast<float>(dae::Settings::window_height - 16));

	gameObject->AddComponent<dae::RenderComponent>(gameObject.get());
	gameObject->GetComponent<dae::RenderComponent>()->SetTexture("galaga-fighter.png");

	gameObject->SetParent(GetGameObject());

	dae::SceneManager::GetInstance().GetActiveScene()->Add(std::move(gameObject));
}