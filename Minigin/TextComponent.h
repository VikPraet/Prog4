#pragma once
#include "RenderComponent.h"

#include "Font.h"
#include <string>
#include <SDL_pixels.h>

namespace dae
{
	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(dae::GameObject* gameObject, const std::string& text, std::unique_ptr<dae::Font> font);
		TextComponent(dae::GameObject* gameObject, std::unique_ptr<dae::Font> font);
		~TextComponent() = default;

		void Update() override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	private:
		bool m_needsUpdate;
		SDL_Color m_Color{255, 255, 255, 255};
		std::string m_text;
		std::unique_ptr<Font> m_font;
		RenderComponent* m_RenderComponent;
	};
}
