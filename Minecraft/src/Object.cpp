#include "Object.h"

// objects are not used yet so commenting out

// std::vector<GLuint> Object::m_VBOS;
// std::vector<GLuint> Object::m_VAOS;
// std::vector<GLuint> Object::m_EBOS;

Object::Object()
{

}


void Object::DestroyObjects()
{
	for (auto& it : m_VAOS)
	{
		glDeleteVertexArrays(1, &it);
	}
}
