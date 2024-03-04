#include <stdexcept>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include <format>
#include <numbers>

namespace dae
{
#pragma region Texture

	void TextureComponent::Update(float)
	{
		if (m_pOwnerGlobalTransform == nullptr) //Check if the transform pointer is set
		{
			m_pOwnerGlobalTransform = GetOwner()->GetGlobalTransform(); //This will only happen the first time
		}
	}

	void TextureComponent::Render() const
	{
		if (m_texture == nullptr)
		{
			return;
		}
		const float textureWidthOffset = m_texture->GetSize().x / 2.f;
		const float textureHeightOffset = m_texture->GetSize().y / 2.f;
		if (m_pOwnerGlobalTransform == nullptr ) //Check if the transform pointer is set
		{
			Renderer::GetInstance().RenderTexture(*m_texture, -textureWidthOffset, -textureHeightOffset);
		}
		else //transform pointer set
		{
			const auto& pos = m_pOwnerGlobalTransform->GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x - textureWidthOffset, pos.y - textureHeightOffset);
		}
	}

	void TextureComponent::SetTexture(const std::string& filename)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(filename);
	}

	void TextureComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
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

	void TextComponent::Update(float)
	{
		if (m_needsUpdate)
		{
			if (m_pOwnerTexture == nullptr) //Check if the transform pointer is set
			{
				m_pOwnerTexture = GetOwner()->GetComponent<TextureComponent>(); //This will only happen the first time
			}
			else
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
				m_pOwnerTexture->SetTexture(std::make_shared<Texture2D>(texture));
				m_needsUpdate = false;
			}
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
			m_Delay += deltaT;
			++m_Count;
			if (m_Delay >= m_MaxTimeBetweenUpdates)
			{
				m_LastFPS = m_Count / m_MaxTimeBetweenUpdates;

				std::string printString = std::format("{:.1f} FPS", m_LastFPS);
				m_pOwnerText->SetText(printString);

				m_Count = 0;
				m_Delay -= m_MaxTimeBetweenUpdates;
			}
		}
	}

#pragma endregion
#pragma region Encircle
	void Encircle::Update(float deltaT)
	{	
		const float twoPi{ static_cast<float>(std::numbers::pi) * 2 };
		m_Angle += m_RotationsPerSec * deltaT;

		if (m_Angle >= twoPi)
		{
			m_Angle -= twoPi;
		}

		m_Offset.x = m_Distance * glm::cos(m_Angle);
		m_Offset.y = m_Distance * glm::sin(m_Angle);

		GetOwner()->SetLocalPosition(glm::vec3(m_Offset.x, m_Offset.y, 0.f));	
	}
}
#pragma endregion