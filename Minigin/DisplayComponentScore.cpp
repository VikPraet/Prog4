#include "DisplayComponentScore.h"
#include "GameObject.h"
#include "ScoreComponent.h"

galaga::DisplayComponentScore::DisplayComponentScore(dae::GameObject* gameObject)
	: BaseComponent(gameObject), Observer()
{
	m_TextComponent = GetGameObject()->GetComponent<dae::TextComponent>();
}

void galaga::DisplayComponentScore::Notify(const dae::Event& event, dae::Subject* subject)
{
	switch (event)
	{
	case dae::Initialize:
	case dae::OnScoreChange:
		UpdateScoreDisplay(subject);
		break;
	}
}

void galaga::DisplayComponentScore::UpdateScoreDisplay(dae::Subject* subject)
{
	const auto scoreComponent = subject->GetParent()->GetComponent<ScoreComponent>();
	const int currentScore = scoreComponent->GetCurrentScore();
	m_TextComponent->SetText(m_BaseText + std::to_string(currentScore));
}