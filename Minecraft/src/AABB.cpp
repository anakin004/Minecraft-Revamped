#include "AABB.h"


AABB::AABB(const glm::vec3& ColliderMinCornerPos, float width, float height)
	: minX(ColliderMinCornerPos.x), minY(ColliderMinCornerPos.y), minZ(ColliderMinCornerPos.z),
	  maxX(ColliderMinCornerPos.x + width), maxY(ColliderMinCornerPos.y + height), maxZ(ColliderMinCornerPos.z + width)
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

bool AABB::collides(const AABB& collider)
{

}
