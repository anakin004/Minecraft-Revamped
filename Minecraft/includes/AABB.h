#pragma once

#include <glm/glm.hpp>

class AABB {
public:

	AABB(const glm::vec3& ColliderMinCornerPos)
		: minX(ColliderMinCornerPos.x), minY(ColliderMinCornerPos.y), minZ(ColliderMinCornerPos.z),
		  maxX(ColliderMinCornerPos.x + 1.0f), maxY(ColliderMinCornerPos.y + 1.0f), maxZ(ColliderMinCornerPos.z + 1.0f)
	{
	}

private:

	// positions for the box collider
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
};