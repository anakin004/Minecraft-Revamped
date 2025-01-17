#include "Phys.h"

std::vector<Block> broadBlocks BroadPhase(const glm::vec3& minPos, const glm::vec3& maxPos)
{
        World& world = World::GetWord();

        std::vector<Block> blocks;

        // assumming max position is greater than minPos
        blocks.reserve((maxPos.x - minPos.x) * (maxPos.y - minPos.y) * (maxPos.z - minPos.z));

        for(float z = minPos.z ; z < maxPos.z ; z += 1.0f)
        {
                for( float x = minPos.x; x < maxPos.x; x += 1.0f)
                {
                        for( float y = minPos.y; y < maxPos.y ; y += 1.0f)
                        {
                                uvec& chunkData = world.GetChunkData(minPos.x, minPos.y, minPos.z);
                                if( !chunkData.empty() )
                                {
                                        blocks.emplace_back(chunkData[int(z * CHUNK_SIZE * CHUNK_SIZE) +
                                                        int(x * CHUNK_SIZE) + int(y)]);

                                }

                        }

                }
        }

}
