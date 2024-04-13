#pragma once
#include "RenderComponent.h"

#include "Font.h"
#include <string>

class TextComponent final : public BaseComponent
{
public:
	TextComponent(dae::GameObject* gameObject, const std::string& text, std::unique_ptr<dae::Font> font);
	TextComponent(dae::GameObject* gameObject, std::unique_ptr<dae::Font> font);
	~TextComponent() = default;

	void Update() override;

	void SetText(const std::string& text);

private:
	bool m_needsUpdate;
	std::string m_text;
	std::unique_ptr<dae::Font> m_font;
	RenderComponent* m_RenderComponent;
};

