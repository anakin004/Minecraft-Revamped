#include "AABB.h"


AABB::AABB(const glm::vec3& ColliderMinCornerPos)
	: minX(ColliderMinCorner.x), minY(ColliderMinCorner.y), minZ(ColliderMinCorner.z),
	  maxX(ColliderMinCorner.x + 1.0f), maxY(ColliderMinCorner.y + 1.0f), maxZ(ColliderMinCorner.z + 1.0f)
{
}


bool AABB::collides(const glm::vec3& point)
{
	if(point.x >= minX && point.x <= maxX &&
	   point.y >= minY && point.y <= maxY &&
    	   point.z >= minZ && point.z <= maxZ
	  )
     	{
		// collides
		return true;
	}
	
	// doesnt collide return false 
	return false;	

}

bool AABB::collides(float x, float y, float z)
{
	return collides(glm::vec3(x,y,z));
}
