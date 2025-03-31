#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
private:
	glm::vec3 Pos;
	glm::quat Rotation;
};