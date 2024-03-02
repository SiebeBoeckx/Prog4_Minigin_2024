#include <stdexcept>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

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
				//const float roundedNum = std::round(m_NrOfFrames * 10) / 10; // Round to 1 digit after the decimal point
				std::string printString = std::to_string(m_LastFPS);

				// Set precision to 1 decimal place
				size_t decimalPos = printString.find('.');
				if (decimalPos != std::string::npos && printString.length() > decimalPos + 2) {
					printString = printString.substr(0, decimalPos + 2);
				}

				printString += " FPS";
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
		if (m_pOwner == nullptr)
		{
			m_pOwner = GetOwner();
		}
		else
		{
			float multiplier{ 1 };//+ for anti-clockwise rotation, + for clockwise, could be variable
			if (m_isRotatingClockwise)
			{
				multiplier = -1;
			}

			m_Time += deltaT;
			if (m_Time >= m_SecPerRotation)
			{
				m_Time -= m_SecPerRotation;
			}

			m_Offset.x = m_Distance * cosf(multiplier * (static_cast<float>(2 * M_PI) / m_SecPerRotation) * m_Time);
			m_Offset.y = m_Distance * sinf(multiplier * (static_cast<float>(2 * M_PI) / m_SecPerRotation) * m_Time);

			m_pOwner->SetLocalPosition(glm::vec3(m_Offset.x, m_Offset.y, 0.f));
			m_pOwner->SetPositionDirty();
		}
	}
}
#pragma endregion