#include "DisplayComponentScore.h"
#include "GameObject.h"
#include "ScoreComponent.h"

galaga::DisplayComponentScore::DisplayComponentScore(dae::GameObject* gameObject, ScoreComponent* scoreComponent)
	: BaseComponent(gameObject)
{
	m_TextComponent = GetGameObject()->GetComponent<dae::TextComponent>();

	if (scoreComponent)
	{
		scoreComponent->OnScoreChanged().AddListener(this, &DisplayComponentScore::OnScoreChanged);
	}
}

void galaga::DisplayComponentScore::OnScoreChanged(int currentScore)
{
	if (m_TextComponent) m_TextComponent->SetText(m_BaseText + std::to_string(currentScore));
}
