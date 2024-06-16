#include "ScoreDisplay.h"
#include "ScoreManager.h"

galaga::ScoreDisplay::ScoreDisplay(dae::GameObject* gameObject)
	: BaseComponent(gameObject), m_ScoreTextComponent(nullptr)
{
	ScoreManager::GetInstance().OnScoreChanged.AddListener(this, &ScoreDisplay::OnScoreChanged);

	if(!m_ScoreTextComponent) gameObject->GetComponent<dae::TextComponent>();
}

void galaga::ScoreDisplay::OnComponentAdded(BaseComponent* component)
{
	if(m_ScoreTextComponent) return;
	if(const auto textComponent = dynamic_cast<dae::TextComponent*>(component))
	{
		m_ScoreTextComponent = textComponent;
		m_ScoreTextComponent->SetText("0");
	}
}

void galaga::ScoreDisplay::OnScoreChanged(int score)
{
	if(m_ScoreTextComponent) m_ScoreTextComponent->SetText(std::to_string(score));
}
