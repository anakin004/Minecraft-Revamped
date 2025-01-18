#include "Phys.h"

std::vector<glm::vec3> BroadPhase(const glm::vec3& minPos, const glm::vec3& maxPos)
{
        World& world = World::GetWorld();

        // uvec is unsigned int vector, refer to BLOCKS enum in Blocks.h
        std::vector<glm::vec3> blocks;

        // assumming max position is greater than minPos
        blocks.reserve((maxPos.x - minPos.x) * (maxPos.y - minPos.y) * (maxPos.z - minPos.z));

        for(float x = minPos.x ; x < maxPos.x ; x += 1.0f)
        {
                for( float z = minPos.z; z < maxPos.z; z += 1.0f)
                {
                        for( float y = minPos.y; y < maxPos.y ; y += 1.0f)
                        {
                            int chunkX = std::floor(x / 16);
                            int chunkY = std::floor(y / 16);
                            int chunkZ = std::floor(z / 16);

                            uvec& chunkData = world.GetChunkData(chunkX, chunkY, chunkZ);
                                    
                            if( chunkData.size() == CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE )
                            {
                                int xIdx = x - chunkX * 16;
                                int yIdx = y - chunkY * 16;
                                int zIdx = z - chunkZ * 16;

                                int vecIndex = xIdx * CHUNK_SIZE * CHUNK_SIZE + zIdx * CHUNK_SIZE + yIdx;

                                // this is safe and avoids safety checks with .at()
                                unsigned int state = chunkData[vecIndex];

                                if (state != Blocks::AIR)
                                {
                                    blocks.emplace_back(x,y,z);
                                }


                            }

                        }

                }
        }
        //std::cout << blocks.size() << '\n';

        return blocks;

}


std::vector<ColliderResult> NarrowPhase(const std::vector<glm::vec3>& blocks, glm::vec3& playerPosition, const AABB& playerCollider)
{
    std::vector<ColliderResult> collisions;

    for (const glm::vec3& block : blocks)
    {
        // block coords start at bottom left
        glm::vec3 closestPoint = {
            std::max(block.x, std::min(playerPosition.x, block.x + 1.0f)),
            std::max(block.y, std::min(playerPosition.y, block.y + 1.0f)),
            std::max(block.z, std::min(playerPosition.z, block.z + 1.0f)),
        };

        float dx = closestPoint.x - playerPosition.x;                   // 1.8f is height of player, divide by 2 to get middle
        float dy = closestPoint.y - (playerPosition.y - /* work around for now */ 1.8f / 2.f);
        float dz = closestPoint.z - playerPosition.z;



        bool collision = playerCollider.collides(closestPoint.x, closestPoint.y, closestPoint.z);

        if (collision)
        {
            
            float overlapY = 1.8f/2 - std::abs(dy);
            glm::vec3 normal{ 0.f, (dy > 0) - (dy < 0), 0.f };
            
            // overlap xz 0 for now, just checking y 
            collisions.emplace_back(normal, block, overlapY, 0.0f);
        }
    }

    return collisions;;

}