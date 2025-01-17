#include "Ray.h"
#include "World.h"
#include "Blocks.h"

Ray::Ray(const glm::vec3& start)
: m_StartPosition(start)
{
}

// for now ray casting will just return wether it hits a block or not

bool Ray::Cast(const glm::vec3& direction, float stepSize, float maxDist)
{
	// copy start position so we compare distance
	glm::vec3 position = m_StartPosition;

	// direction should be normalized

	World& world = World::GetWorld();
	bool hitBlock = false;

	while (!hitBlock)
	{

		int chunkX = static_cast<int>(std::floor(position.x / 16.f));
		int chunkY = static_cast<int>(std::floor(position.y / 16.f));
		int chunkZ = static_cast<int>(std::floor(position.z / 16.f));
		uvec& chunkData = world.GetChunkData(chunkX, chunkY, chunkZ);

		// blocks were pushed into uvec using z as outer loop, x as next inner, then y as final
		// z * CHUNK_SIZE * CHUNK_SIZE + x * CHUNK_SIZE + y

		int index = static_cast<int>(std::floor(position.z)) * CHUNK_SIZE * CHUNK_SIZE + \
			static_cast<int>(std::floor(position.x)) * CHUNK_SIZE + \
			static_cast<int>(std::floor(position.y));

		if (chunkData.at(index) != Blocks::AIR)
		{
			hitBlock = true;
		}

		else
		{
			if (glm::distance(position, m_StartPosition) >= maxDist)
				break;
		}

	}


	return hitBlock;
	

}

