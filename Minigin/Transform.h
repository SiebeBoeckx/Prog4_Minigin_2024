#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(glm::vec3 pos);

		Transform operator+(const Transform& other) const {
			Transform result;
			// Perform addition of positions
			result.m_position = this->m_position + other.m_position;
			return result;
		}

	private:
		glm::vec3 m_position{};
	};
}
