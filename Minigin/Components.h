#pragma once
#include <glm/glm.hpp>
#include <string>
#include "GameObject.h"

namespace dae
{
	class Texture2D;
	class Font;

	class Component
	{
	public:
		//Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;


		virtual void BeginPlay() {};
		virtual void Update(float) {};
		virtual void FixedUpdate(float) {};
		virtual void Render() const {};
		virtual void SetPosition(float, float, float) {};
		virtual void SetPosition(glm::vec3) {};

		GameObject* GetOwner() const { return m_pOwner; };
		void MarkForDelete() { m_MarkForDelete = true; };
		const bool GetMarkForDelete() const { return m_MarkForDelete; };

	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {};
	private:
		GameObject* m_pOwner{ nullptr };
		bool m_MarkForDelete{ false };
	};

	class TextureComponent final : public Component
	{
	public:
		TextureComponent(GameObject* pOwner)
			:Component(pOwner)
		{};

		void Update(float) override;
		void Render() const override;
		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D>& texture);
		std::shared_ptr<Texture2D> GetTexturePtr() const { return m_texture; };
	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
		const Transform* m_pOwnerGlobalTransform{ nullptr };
	};

	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font);
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color);

		void Update(float) override;

		void SetText(const std::string& text);
		void SetColor(Uint8 r, Uint8 g, Uint8 b);
		void SetColor(const SDL_Color& color);
	private:
		bool m_needsUpdate; //Only update at initialize or if text changes
		std::string m_text;
		std::shared_ptr<Font> m_font{ nullptr };
		SDL_Color m_Color{ 255,255,255 };
		TextureComponent* m_pOwnerTexture{ nullptr };
	};

	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner)
			:Component(pOwner)
		{};

		void Update(float deltaT) override;
		float GetNrOfFrames() const { return m_LastFPS; };

	private:
		int m_Count{ 0 };
		const float m_MaxTimeBetweenUpdates{ 0.3f };
		float m_Delay{ 0.f };
		float m_LastFPS{ 0.f };
		TextComponent* m_pOwnerText{ nullptr };
	};

	class Encircle final : public Component
	{
	public:
		Encircle(GameObject* pOwner, float distance = 0.f, float secPerRotation = 1.f, bool clockwise = true)
			:Component(pOwner)
			, m_Distance(distance)
			, m_SecPerRotation(secPerRotation)
			, m_isRotatingClockwise(clockwise)
		{};

		void Update(float deltaT) override;
	private:
		glm::vec3 m_Offset{};
		float m_Distance{};
		float m_SecPerRotation{};
		float m_Time{};
		bool m_isRotatingClockwise{};
		GameObject* m_pOwner{ nullptr };
	};
}