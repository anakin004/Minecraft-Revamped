#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <glm/glm.hpp>

#include "Chunk.h"
#include "Hash.h"
#include "Shader.h"
#include "util.h"

using cTuple = std::tuple<int, int, int>;

class World
{


public:
	World();
	~World();

	std::vector<unsigned int> GetChunkData(int chunkX, int chunkY, int chunkZ);
	void Update(glm::vec3 camPos, Shader* shader);


	static World& GetWorld() {
		static World w;
		return w;
	}


private:

	// for chunk generation
	Thread::ThreadPool m_ThreadPool;

	std::unordered_map<cTuple, Chunk> m_Chunks;
	std::queue<glm::vec3> m_Queue;

	int m_RenderDistance = 12;
	int m_RenderHeight = 1;

	unsigned int m_ChunksLoading;
	unsigned int m_NumChunks;
	unsigned int m_NumChunksRendered;

	int m_PrevCamX;
	int m_PrevCamY;
	int m_PrevCamZ;
};