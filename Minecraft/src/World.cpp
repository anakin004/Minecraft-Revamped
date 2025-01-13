#include "World.h"
#include "FMath.h"

#include <iostream>



World::World() : m_ThreadPool(4), m_RenderHeight(1), 
m_ChunksLoading(0), m_NumChunks(0), m_NumChunksRendered(0), m_PrevCamX(0), m_PrevCamY(20), m_PrevCamZ(0)
{
}

World::~World()
{
}

void World::Update(glm::vec3 camPos, Shader* shader)
{

	// we want to floor if we are negative, since int divsion will round up
	int camChunkX = camPos.x < 0 ? ftoi_fast(camPos.x / CHUNK_SIZE) : camPos.x / CHUNK_SIZE;
	int camChunkY = camPos.y < 0 ? ftoi_fast(camPos.y / CHUNK_SIZE) : camPos.y / CHUNK_SIZE;
	int camChunkZ = camPos.z < 0 ? ftoi_fast(camPos.z / CHUNK_SIZE) : camPos.z / CHUNK_SIZE;

	// Check if camera moved to new chunk
	if (camChunkX != m_PrevCamX || camChunkY != m_PrevCamY || camChunkZ != m_PrevCamZ)
	{


		m_PrevCamX = camChunkX;
		m_PrevCamY = camChunkY;
		m_PrevCamZ = camChunkZ;

		// chunk we are at
		/*
		
		we want to prioritize the chunk we are at currently as soon as we can in a queue
		if we dont and we rapidly move to a new area we would be generating chunks possibly out of render distance

		*/
		m_Queue = {};
		if (m_Chunks.find({ camChunkX, camChunkY, camChunkZ }) == m_Chunks.end())
			m_Queue.push({ camChunkX, camChunkY, camChunkZ });

		for (int renderStep = 0; renderStep < m_RenderDistance; renderStep++)
		{


			// middle 
			for (int y = 0; y <= m_RenderHeight; y++)
			{
				m_Queue.push({ camChunkX,              y, camChunkZ + renderStep });
				m_Queue.push({ camChunkX + renderStep, y, camChunkZ });
				m_Queue.push({ camChunkX,              y, camChunkZ - renderStep });
				m_Queue.push({ camChunkX - renderStep, y, camChunkZ });

				if (y > 0)
				{
					m_Queue.push({ camChunkX,              -y, camChunkZ + renderStep });
					m_Queue.push({ camChunkX + renderStep, -y, camChunkZ });
					m_Queue.push({ camChunkX,              -y, camChunkZ - renderStep });
					m_Queue.push({ camChunkX - renderStep, -y, camChunkZ });
				}
			}


			// corners
			for (int y = 0; y <= m_RenderHeight; y++)
			{
				m_Queue.push({ camChunkX + renderStep, y, camChunkZ + renderStep });
				m_Queue.push({ camChunkX + renderStep, y, camChunkZ - renderStep });
				m_Queue.push({ camChunkX - renderStep, y, camChunkZ + renderStep });
				m_Queue.push({ camChunkX - renderStep, y, camChunkZ - renderStep });

				if (y > 0)
				{
					m_Queue.push({ camChunkX + renderStep, -y, camChunkZ + renderStep });
					m_Queue.push({ camChunkX + renderStep, -y, camChunkZ - renderStep });
					m_Queue.push({ camChunkX - renderStep, -y, camChunkZ + renderStep });
					m_Queue.push({ camChunkX - renderStep, -y, camChunkZ - renderStep });
				}
			}

			
			// edges
			for (int edge = 1; edge < renderStep; edge++)
			{
				for (int y = 0; y <= m_RenderHeight; y++)
				{
					m_Queue.push({ camChunkX + edge, y, camChunkZ + renderStep });
					m_Queue.push({ camChunkX - edge, y, camChunkZ + renderStep });

					m_Queue.push({ camChunkX + renderStep, y, camChunkZ + edge });
					m_Queue.push({ camChunkX + renderStep, y, camChunkZ - edge });

					m_Queue.push({ camChunkX + edge, y, camChunkZ - renderStep });
					m_Queue.push({ camChunkX - edge, y, camChunkZ - renderStep });

					m_Queue.push({ camChunkX - renderStep, y, camChunkZ + edge });
					m_Queue.push({ camChunkX - renderStep, y, camChunkZ - edge });

					if (y > 0)
					{
						m_Queue.push({ camChunkX + edge, -y, camChunkZ + renderStep });
						m_Queue.push({ camChunkX - edge, -y, camChunkZ + renderStep });

						m_Queue.push({ camChunkX + renderStep, -y, camChunkZ + edge });
						m_Queue.push({ camChunkX + renderStep, -y, camChunkZ - edge });

						m_Queue.push({ camChunkX + edge, -y, camChunkZ - renderStep });
						m_Queue.push({ camChunkX - edge, -y, camChunkZ - renderStep });

						m_Queue.push({ camChunkX - renderStep, -y, camChunkZ + edge });
						m_Queue.push({ camChunkX - renderStep, -y, camChunkZ - edge });
					}
				}
			}


		}
	}
	else if (m_ChunksLoading == 0 && !m_Queue.empty())
	{
		// proccess next item since queue no empty
		glm::vec3 next = m_Queue.front();
		m_Queue.pop();

		cTuple chunkTuple{ next.x, next.y, next.z };

		if (m_Chunks.find(chunkTuple) == m_Chunks.end())
		{
			m_Chunks.try_emplace(chunkTuple,
				CHUNK_SIZE, next, m_ThreadPool
			);
		}
	}

	m_ChunksLoading = 0;
	m_NumChunks = 0;
	m_NumChunksRendered = 0;
	for (auto it = m_Chunks.begin(); it != m_Chunks.end(); )
	{
		m_NumChunks++;

		bool chunkReady = it->second.GetReadyStatus();

		if (!chunkReady)
			m_ChunksLoading++;

		glm::vec3& cPos = it->second.GetPos();

		int chunkX = cPos.x;
		int chunkY = cPos.y;
		int chunkZ = cPos.z;


		if (chunkReady && (abs(chunkX - camChunkX) > m_RenderDistance ||
			abs(chunkY - camChunkY) > m_RenderDistance ||
			abs(chunkZ - camChunkZ) > m_RenderDistance
			))
		{
			it->second.SetRender(false);
			m_NumChunksRendered--;
			++it;
		}
		else
		{
			it->second.OnUpdate();
			it->second.SetRender(true);
			it->second.Render(shader);
			m_NumChunksRendered++;
			++it;
		}
	}
}

std::vector<unsigned int>& World::GetChunkData(int chunkX, int chunkY, int chunkZ)
{
	cTuple chunkTuple{ chunkX, chunkY, chunkZ };
	if (m_Chunks.find(chunkTuple) == m_Chunks.end())
	{
		return std::vector<unsigned int>{};
	}
	else
	{
		return m_Chunks[chunkTuple].GetData();
	}
}
