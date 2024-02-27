#pragma once
#include "RenderComponent.h"

#include "Font.h"
#include <string>

class TextComponent final : public Component
{
public:
	TextComponent(const std::shared_ptr<dae::GameObject>& gameObject, const std::string& text, std::shared_ptr<dae::Font> font);
	TextComponent(const std::shared_ptr<dae::GameObject>& gameObject, std::shared_ptr<dae::Font> font);
	~TextComponent() = default;

	void Update() override;
	void Render() const override;

	void SetText(const std::string& text);

private:
	bool m_needsUpdate;
	std::string m_text;
	std::shared_ptr<dae::Font> m_font;
	RenderComponent* m_RenderComponent;
};

