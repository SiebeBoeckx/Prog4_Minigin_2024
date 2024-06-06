#include <SDL.h>
#include "Texture2D.h"
#include <iostream>

dae::Texture2D::~Texture2D()
{
	if (m_texture)
	{
		SDL_DestroyTexture(m_texture);
		//if (const char* error = SDL_GetError(); *error != '\0')
		//{
		//	std::cerr << "SDL_DestroyTexture error: " << error << std::endl;
		//	SDL_ClearError(); // Clear the error message after logging
		//}
		m_texture = nullptr;
	}
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_texture = texture;
}
