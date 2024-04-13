#include "TextComponent.h"

#include <SDL_ttf.h>
#include <stdexcept>

#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"

TextComponent::TextComponent(dae::GameObject* gameObject, const std::string& text, std::unique_ptr<dae::Font> font)
	: BaseComponent(gameObject), m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_RenderComponent(nullptr)
{
}

TextComponent::TextComponent(dae::GameObject* gameObject, std::unique_ptr<dae::Font> font)
	: BaseComponent(gameObject), m_needsUpdate(true), m_text("Text"), m_font(std::move(font)), m_RenderComponent(nullptr)
{
}

void TextComponent::Update()
{
	if (m_RenderComponent == nullptr) m_RenderComponent = GetGameObject()->GetComponent<RenderComponent>();

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

		if(m_RenderComponent) m_RenderComponent->SetTexture(std::make_unique<dae::Texture2D>(texture));

		m_needsUpdate = false;
	}
}

void TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}
