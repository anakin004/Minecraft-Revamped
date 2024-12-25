#include "PlayerObj.h"
#include "Model.h"

ModelLoader::Model PlayerObj::m_PlayerModel("resources/models/steve/scene.gltf");

PlayerObj::PlayerObj() : m_Position(glm::vec3(0.f, 7.f, 0.f)), m_Orientation(glm::vec3(0.f, 0.f, 1.f))
{

}
