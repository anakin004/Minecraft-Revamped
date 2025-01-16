#pragma once
#include <glm/glm.hpp>



class Ray
{

	public:
		Ray(const glm::vec3& start);
		
		// unsure of return type
		// may return block or boolean

		bool Cast(glm::vec3& direction, int numSteps, float stepSize, float maxDist);
		

	private:

		glm::vec3 m_StartPosition;


};

