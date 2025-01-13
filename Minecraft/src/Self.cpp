#include "Self.h"



Self::Self(const glm::vec3& pos)
	: m_Position(pos), m_Orientation(0.0f, 0.0f, -1.0f), m_Rotation(90.0f)
{
}

void Self::OnUpdate(float deltaTime)
{
}
