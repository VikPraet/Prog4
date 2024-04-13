#include "DisplayComponentScore.h"
#include "GameObject.h"
#include "ScoreComponent.h"

dae::DisplayComponentScore::DisplayComponentScore(dae::GameObject* gameObject)
	: BaseComponent(gameObject), Observer()
{
	m_TextComponent = GetGameObject()->GetComponent<TextComponent>();
}

void dae::DisplayComponentScore::Notify(const Event& event, Subject* subject)
{
	switch (event)
	{
	case Initialize:
	case OnScoreChange:
		UpdateScoreDisplay(subject);
		break;
	}
}

void dae::DisplayComponentScore::UpdateScoreDisplay(Subject* subject)
{
	const auto scoreComponent = subject->GetParent()->GetComponent<ScoreComponent>();
	const int currentScore = scoreComponent->GetCurrentScore();
	m_TextComponent->SetText(m_BaseText + std::to_string(currentScore));
}