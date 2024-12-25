#include "Chunk.h"
#include "World.h"
#include "Blocks.h"
#include "util.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <simplex/OpenSimplexNoise.hh>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



using uvec = std::vector<unsigned int>;


Chunk::Chunk(unsigned int chunkSize, glm::vec3 chunkPos, Thread::ThreadPool& tp)
{
	m_ChunkSize = chunkSize;
	m_ChunkPos = chunkPos;
	m_WorldPos = glm::vec3(chunkPos.x * chunkSize, chunkPos.y * chunkSize, chunkPos.z * chunkSize);
	m_Ready = false;
	m_Generated = false;

	// when we generate chunk we should default it to render, depending on render dist this flag will change, but generally it will be close enough
	m_Render = true;
 

	tp.enqueueTask(std::bind(&Chunk::GenerateChunk, this));

}


Chunk::Chunk()
{
	m_ChunkSize = CHUNK_SIZE;
	m_ChunkPos = glm::vec3(0.f,0.f,0.f);
	m_WorldPos = glm::vec3(0.f, 0.f, 0.f);
	m_Ready = false;
	m_Generated = false;
}

Chunk::~Chunk()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Chunk::GenerateChunk()
{

	World &w = World::GetWorld();

	MakeChunkData(m_ChunkPos.x, m_ChunkPos.y, m_ChunkPos.z, CHUNK_SIZE, &m_ChunkData);


	//uvec northChunk, southChunk, eastChunk, westChunk, upChunk, downChunk;
 

	/*
	
	for some reason this is so slow ( calling get chunk data instead of creating surrounding each time )
	i will need to call get chunk data and fix it as with block placing and destroying we will need to change 
	chunks, thus neighboring chunks getting created need access to that

	*/
	
	
 	uvec northChunk = w.GetChunkData(m_ChunkPos.x, m_ChunkPos.y, m_ChunkPos.z - 1);
 	uvec southChunk = w.GetChunkData(m_ChunkPos.x, m_ChunkPos.y, m_ChunkPos.z + 1);
 	uvec eastChunk  = w.GetChunkData(m_ChunkPos.x + 1, m_ChunkPos.y, m_ChunkPos.z);
	uvec westChunk  = w.GetChunkData(m_ChunkPos.x - 1, m_ChunkPos.y, m_ChunkPos.z);
	uvec upChunk    = w.GetChunkData(m_ChunkPos.x, m_ChunkPos.y + 1, m_ChunkPos.z);
	uvec downChunk  = w.GetChunkData(m_ChunkPos.x, m_ChunkPos.y + 1, m_ChunkPos.z);

	

	      
	if(northChunk.empty())
	MakeChunkData(m_ChunkPos.x,		m_ChunkPos.y,		m_ChunkPos.z - 1, CHUNK_SIZE, &northChunk);
	if(southChunk.empty())
	MakeChunkData(m_ChunkPos.x,		m_ChunkPos.y,		m_ChunkPos.z + 1, CHUNK_SIZE, &southChunk);
	if (eastChunk.empty())
	MakeChunkData(m_ChunkPos.x + 1, m_ChunkPos.y,		m_ChunkPos.z,	  CHUNK_SIZE, &eastChunk);
	if(westChunk.empty())
	MakeChunkData(m_ChunkPos.x - 1, m_ChunkPos.y,		m_ChunkPos.z,	  CHUNK_SIZE, &westChunk);
	if(upChunk.empty())
	MakeChunkData(m_ChunkPos.x,		m_ChunkPos.y + 1,	m_ChunkPos.z,	  CHUNK_SIZE, &upChunk);
	if(downChunk.empty())
	MakeChunkData(m_ChunkPos.x,		m_ChunkPos.y - 1,	m_ChunkPos.z,	  CHUNK_SIZE, &downChunk);



	unsigned int currentVertex = 0;
	for (char x = 0; x < CHUNK_SIZE; x++)
	{
		for (char z = 0; z < CHUNK_SIZE; z++)
		{
			for (char y = 0; y < CHUNK_SIZE; y++)
			{
				int index = x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y;
				if (m_ChunkData[index] == 0)
					continue;

				const Block* block = &Blocks::blocks[m_ChunkData[index]];


				{
					int northBlock = -1;



					/*
					
					if we are at the far north end of the chunk, the north face will depend on whether or not
					the block in the next chunk to the north is solid or not
					the same idea will apply to other blocks that we check in this function
					
					when we check the southern block face, if the current z component is at CHUNK_SIZE
					it will depend on the block in the chunk south of the current chunk

					*/
					if (z == 0)
					{
						int northIndex = x * CHUNK_SIZE * CHUNK_SIZE + (CHUNK_SIZE - 1) * CHUNK_SIZE + y;
						northBlock = northChunk[northIndex];
					}
					else
					{
						int northIndex = x * CHUNK_SIZE * CHUNK_SIZE + (z - 1) * CHUNK_SIZE + y;
						northBlock = m_ChunkData[northIndex];
					}

					if (northBlock == 0)
					{
						m_Vertices.push_back(Vertex(x + 1, y + 0, z + 0, block->sideMinX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 0, y + 0, z + 0, block->sideMaxX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 1, y + 1, z + 0, block->sideMinX, block->sideMaxY));
						m_Vertices.push_back(Vertex(x + 0, y + 1, z + 0, block->sideMaxX, block->sideMaxY));

						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 3);
						m_Indices.push_back(currentVertex + 1);
						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 2);
						m_Indices.push_back(currentVertex + 3);
						currentVertex += 4;
					}
				}

				{
					int southBlock  = -1;
					if (z < CHUNK_SIZE - 1)
					{
						int southIndex = x * CHUNK_SIZE * CHUNK_SIZE + (z + 1) * CHUNK_SIZE + y;
						southBlock = m_ChunkData[southIndex];
					}
					else
					{
						int southIndex = x * CHUNK_SIZE * CHUNK_SIZE + 0 * CHUNK_SIZE + y;
						southBlock = southChunk[southIndex];
					}
					if (southBlock == 0)
					{
						m_Vertices.push_back(Vertex(x + 0, y + 0, z + 1, block->sideMinX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 1, y + 0, z + 1, block->sideMaxX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 0, y + 1, z + 1, block->sideMinX, block->sideMaxY));
						m_Vertices.push_back(Vertex(x + 1, y + 1, z + 1, block->sideMaxX, block->sideMaxY));

						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 3);
						m_Indices.push_back(currentVertex + 1);
						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 2);
						m_Indices.push_back(currentVertex + 3);
						currentVertex += 4;
					}
				}

				{
					int westBlock = -1;
					if (x > 0)
					{
						int blockIndex = (x - 1) * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y;
						westBlock = m_ChunkData[blockIndex];
					}
					else
					{
						int blockIndex = (CHUNK_SIZE - 1) *CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y;
						westBlock = westChunk[blockIndex];
					}
					if (westBlock == 0)
					{
						m_Vertices.push_back(Vertex(x + 0, y + 0, z + 0, block->sideMinX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 0, y + 0, z + 1, block->sideMaxX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 0, y + 1, z + 0, block->sideMinX, block->sideMaxY));
						m_Vertices.push_back(Vertex(x + 0, y + 1, z + 1, block->sideMaxX, block->sideMaxY));

						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 3);
						m_Indices.push_back(currentVertex + 1);
						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 2);
						m_Indices.push_back(currentVertex + 3);
						currentVertex += 4;
					}
				}


				{
					int eastBlock = -1;
					if (x < CHUNK_SIZE - 1)
					{
						int blockIndex = (x + 1) * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y;
						eastBlock = m_ChunkData[blockIndex];
					}
					else
					{
						int blockIndex = 0 * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y;
						eastBlock = eastChunk[blockIndex];
					}
					if (eastBlock == 0)
					{
						m_Vertices.push_back(Vertex(x + 1, y + 0, z + 1, block->sideMinX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 1, y + 0, z + 0, block->sideMaxX, block->sideMinY));
						m_Vertices.push_back(Vertex(x + 1, y + 1, z + 1, block->sideMinX, block->sideMaxY));
						m_Vertices.push_back(Vertex(x + 1, y + 1, z + 0, block->sideMaxX, block->sideMaxY));

						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 3);
						m_Indices.push_back(currentVertex + 1);
						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 2);
						m_Indices.push_back(currentVertex + 3);
						currentVertex += 4;
					}
				}


				{
					int bottomBlock = -1;
					if (y > 0)
					{
						int blockIndex = x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + (y - 1);
						bottomBlock = m_ChunkData[blockIndex];
					}
					else
					{
						int blockIndex = x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + (CHUNK_SIZE - 1);
						bottomBlock = downChunk[blockIndex];
					}
					if (bottomBlock == 0)
					{
						m_Vertices.push_back(Vertex(x + 1, y + 0, z + 1, block->bottomMinX, block->bottomMinY));
						m_Vertices.push_back(Vertex(x + 0, y + 0, z + 1, block->bottomMaxX, block->bottomMinY));
						m_Vertices.push_back(Vertex(x + 1, y + 0, z + 0, block->bottomMinX, block->bottomMaxY));
						m_Vertices.push_back(Vertex(x + 0, y + 0, z + 0, block->bottomMaxX, block->bottomMaxY));
						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 3);
						m_Indices.push_back(currentVertex + 1);
						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 2);
						m_Indices.push_back(currentVertex + 3);
						currentVertex += 4;
					}
				}


				{
					int topBlock = -1;
					if (y < CHUNK_SIZE - 1)
					{
						int blockIndex = x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + (y + 1);
						topBlock = m_ChunkData[blockIndex];
					}
					else
					{
						int blockIndex = x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE;
						topBlock = upChunk[blockIndex];
					}
					if (topBlock == 0)
					{
						m_Vertices.push_back(Vertex(x + 0, y + 1, z + 1, block->topMinX, block->topMinY));
						m_Vertices.push_back(Vertex(x + 1, y + 1, z + 1, block->topMaxX, block->topMinY));
						m_Vertices.push_back(Vertex(x + 0, y + 1, z + 0, block->topMinX, block->topMaxY));
						m_Vertices.push_back(Vertex(x + 1, y + 1, z + 0, block->topMaxX, block->topMaxY));

						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 3);
						m_Indices.push_back(currentVertex + 1);
						m_Indices.push_back(currentVertex + 0);
						m_Indices.push_back(currentVertex + 2);
						m_Indices.push_back(currentVertex + 3);
						currentVertex += 4;
					}
				}
			}
		}
	}

	m_Generated.store(true);

}



void Chunk::OnUpdate() 
{
	if (!m_Ready)
	{
		if (m_Generated.load())
		{
			m_NumTriangles = m_Indices.size();

			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);

			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, posX));
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_BYTE, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texGridX));
			glEnableVertexAttribArray(1);

			glGenBuffers(1, &m_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

			glBindVertexArray(0);
			m_Ready = true;
		}
	}
}

void Chunk::Render(Shader* shader)
{
	if (m_Render && m_Ready)
	{
		glBindVertexArray(m_VAO);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_WorldPos);
		shader->SetUniformMat4f("model", model);

		glDrawElements(GL_TRIANGLES, m_NumTriangles, GL_UNSIGNED_INT, 0);
	}


}




void Chunk::MakeChunkData(int chunkX, int chunkY, int chunkZ, int chunkSize, uvec* chunkData)
{


	chunkData->reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

	OSN::Noise<2> surfaceNoise;
	OSN::Noise<3> caveNoise;
	int baseX = chunkX * CHUNK_SIZE;
	int baseY = chunkY * CHUNK_SIZE;
	int baseZ = chunkZ * CHUNK_SIZE;


	constexpr float heightScale = 12.0f;
	constexpr float noiseScale = 0.07f;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			int noiseY = (surfaceNoise.eval((float)(x + baseX) * .1f, (float)(z + baseZ) * .1f) * heightScale) + 20;
			for (int y = 0; y < CHUNK_SIZE; y++)
			{

				float noiseCaves = caveNoise.eval(
					(float)(x + baseX) * noiseScale,
					(float)(y + baseY) * noiseScale,
					(float)(z + baseZ) * noiseScale);

				int numWaterBlocks = noiseY - WATER_LEVEL;

 				// water -> temp, just looks cool for now
				if (y + baseY < WATER_LEVEL && y + baseY > noiseY)
				{
					chunkData->push_back(Blocks::WATER);
				}

				// caves
				else if (y + baseY > noiseY)
					chunkData->push_back(0);
				else if (noiseCaves > .5f)
					chunkData->push_back(0);

				// ground and underground, sand and water stuff is temp
				else if (y + baseY == noiseY && noiseY == WATER_LEVEL)
					chunkData->push_back(Blocks::SAND);


				else if (y + baseY == noiseY - numWaterBlocks - 1 && noiseY < WATER_LEVEL)
					chunkData->push_back(Blocks::SAND);

				else if (y + baseY == noiseY)
					chunkData->push_back(Blocks::GRASS);


				else if (y + baseY > 10)
					chunkData->push_back(Blocks::DIRT);


				else
					chunkData->push_back(Blocks::STONE);
			}
		}
	}
}