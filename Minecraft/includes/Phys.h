#pragma once

#include <glm/glm.hpp>
#include "Blocks.h"
#include "World.h"
#include "AABB.h"

std::vector<glm::vec3> BroadPhase(const glm::vec3& minPos, const glm::vec3& maxPos);
std::vector<glm::vec3> NarrowPhase(const std::vector<glm::vec3>& blocks, glm::vec3& playerPosition, const AABB& playerCollider);
