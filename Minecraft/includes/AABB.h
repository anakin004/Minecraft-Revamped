#pragma once

#include <glm/glm.hpp>

class AABB {
public:

	AABB(const glm::vec3& ColliderMinCornerPos);
	
	bool collides(const glm::vec3& point);
	bool collides(float x, float y, float z);

private:

	// positions for the box collider
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
};
