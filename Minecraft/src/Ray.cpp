#include "Ray.h"



Ray::Ray(const glm::vec3& start)
: m_Position(start)
{
}

// for now ray casting will just return wether it hits a block or not

bool Ray::Cast(const glm::vec3 direction, int numSteps, float stepSize, float maxDist)
{
	

}

