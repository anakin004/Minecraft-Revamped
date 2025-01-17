#pragma once

#include <glm/glm.hpp>
#include "Blocks.h"
#include "World.h"

std::vector<Block> broadBlocks BroadPhase(const glm::vec3& minPos, const glm::vec3& maxPos);
