#ifndef PLAYER_H
#define PLAYER_H

#include<glm/glm.hpp>
#include "Model.h"
#include "AABB.h"

class PlayerObj{

public:
	
	PlayerObj();

	static ModelLoader::Model m_PlayerModel;

protected:

	glm::vec3 m_Position;
	glm::vec3 m_Orientation;

	// might remove since bounding box is based off of player width and height	
	AABB m_Collider;

	float m_Rotation;

};




#endif
