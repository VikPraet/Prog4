#include "TextComponent.h"

#include <SDL_ttf.h>
#include <stdexcept>

#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"

TextComponent::TextComponent(const std::shared_ptr<dae::GameObject>& gameObject, const std::string& text, std::shared_ptr<dae::Font> font)
	: Component(gameObject), m_needsUpdate(true), m_text(text), m_font(std::move(font))
{
}

TextComponent::TextComponent(const std::shared_ptr<dae::GameObject>& gameObject, std::shared_ptr<dae::Font> font)
	: Component(gameObject), m_needsUpdate(true), m_text("Text"), m_font(std::move(font))
{
}

void TextComponent::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);

		if (m_RenderComponent == nullptr) m_RenderComponent = GetGameObject()->GetComponent<RenderComponent>();

		if(m_RenderComponent) m_RenderComponent->SetTexture(std::make_shared<dae::Texture2D>(texture));

		m_needsUpdate = false;
	}
}

void TextComponent::Render() const
{
	if (m_RenderComponent->GetTexture() != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_RenderComponent->GetTexture(), pos.x, pos.y);
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}
