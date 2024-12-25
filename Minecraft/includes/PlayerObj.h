#ifndef PLAYER_H
#define PLAYER_H

#include<glm/glm.hpp>
#include "Model.h"

class PlayerObj{

public:
	
	PlayerObj();

	static ModelLoader::Model m_PlayerModel;

protected:

	glm::vec3 m_Position;
	glm::vec3 m_Orientation;

};




#endif
