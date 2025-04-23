#pragma once

#include "Shader.h"
#include "util.h"
#include "Thread.h"

#include <vector>
#include <glm/glm.hpp>

struct ChunkPosition {
	int x, y, z;

	// equality operator is required for unordered_map/set
	bool operator==(const ChunkPosition& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
};

using uvec = std::vector<unsigned int>;

class Chunk
{
public:

	Chunk();

	Chunk(unsigned int chunkSize, glm::vec3 chunkPos, Thread::ThreadPool& tp);
	~Chunk();

	void GenerateChunk();
	void Render(Shader* shader);
	void OnUpdate();

	inline void SetRender(bool render) { m_Render = render; }
	inline bool GetReadyStatus() { return m_Ready; }
	inline glm::vec3& GetPos() { return m_ChunkPos; }
	inline uvec&  GetData() { return m_ChunkData; }
	inline void SetDirty(bool dirt) { m_Dirty = dirt; }


	void SetBlock(int idx, unsigned int blockType);
	void MakeChunkData(int x, int y, int z, int chunkSize, std::vector<unsigned int>* chunkData);


private:


	uvec m_ChunkData;
	glm::vec3 m_ChunkPos;
	bool m_Ready;
	bool m_Render;
	bool m_Dirty = false;
	std::atomic<bool> m_Generated;

	GLuint m_VAO, m_VBO, m_EBO;

	unsigned int m_ChunkSize;
	unsigned int m_NumTriangles;
	glm::vec3 m_WorldPos;

	std::vector<Vertex> m_Vertices;
	uvec m_Indices;
};