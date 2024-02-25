#include <stdexcept>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

namespace dae
{
#pragma region Render

	void RenderComponent::Update(float)
	{
		if (m_pOwnerTransform == nullptr) //Check if the transform pointer is set
		{
			m_pOwnerTransform = GetOwner()->GetTransform(); //This will only happen the first time
		}
		//or every update if there's no transform, this need optimization but don't know how
	}

	void RenderComponent::Render() const
	{
		const float textureWidthOffset = m_texture->GetSize().x / 2.f;
		const float textureHeightOffset = m_texture->GetSize().y / 2.f;
		if (m_pOwnerTransform == nullptr) //Check if the transform pointer is set
		{
			Renderer::GetInstance().RenderTexture(*m_texture, -textureWidthOffset, -textureHeightOffset);
		}
		else
		{
			const auto& pos = m_pOwnerTransform->GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x - textureWidthOffset, pos.y - textureHeightOffset);
		}
	}

	void RenderComponent::SetTexture(const std::string& filename)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	}

	void RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
	{
		m_texture = texture;
	}
#pragma endregion
#pragma region Text

	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font)
		:Component(pOwner)
		, m_needsUpdate(true)
		, m_text(text)
		, m_font(std::move(font))
	{
	}

	TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
		:Component(pOwner)
		, m_needsUpdate(true)
		, m_text(text)
		, m_font(std::move(font))
		, m_Color(color)
	{
	}

	void TextComponent::Update([[maybe_unused]] float deltaT)
	{
		if (m_needsUpdate)
		{
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_Color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_FreeSurface(surf);
			GetOwner()->GetComponent<RenderComponent>()->SetTexture(std::make_shared<Texture2D>(texture));
			m_needsUpdate = false;
		}
	}

	// This implementation uses the "dirty flag" pattern
	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

	void TextComponent::SetColor(Uint8 r, Uint8 g, Uint8 b)
	{
		m_Color.r = r;
		m_Color.g = g;
		m_Color.b = b;
	}

	void TextComponent::SetColor(const SDL_Color& color)
	{
		m_Color = color;
	}

#pragma endregion
#pragma region FPS

	void dae::FPSComponent::Update(float deltaT)
	{
		if (m_pOwnerText == nullptr) //Check if the text pointer is set
		{
			m_pOwnerText = GetOwner()->GetComponent<TextComponent>(); //This will only happen the first time
		}
		else
		{
			m_NrOfFrames = 1 / deltaT;
			//const float roundedNum = std::round(m_NrOfFrames * 10) / 10; // Round to 1 digit after the decimal point
			std::string printString = std::to_string(m_NrOfFrames);

			// Set precision to 1 decimal place
			size_t decimalPos = printString.find('.');
			if (decimalPos != std::string::npos && printString.length() > decimalPos + 2) {
				printString = printString.substr(0, decimalPos + 2);
			}

			printString += " FPS";
			m_pOwnerText->SetText(printString);
		}
	}

#pragma endregion
}