#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

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
}