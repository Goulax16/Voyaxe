#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp> // Include this for quaternion rotation

class Transform
{
public:
	glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
	glm::quat Rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 ScaleValue{ 1.0f, 1.0f, 1.0f };

public:
	void Translate(const glm::vec3& pos) {
		Position += pos;
	}

	void Rotate(float angle, const glm::vec3& axis) {
		Rotation = glm::rotate(Rotation, glm::radians(angle), axis);
	}

    void Scale(const glm::vec3& scale) {
		ScaleValue += scale;
    }

	void Translate(float x, float y, float z) {
		Position.x += x;
		Position.y += y;
		Position.z += z;
	}

	void Rotate(float angle, float rx, float ry, float rz) {
		glm::vec3 axis(rx, ry, rz);
		Rotation = glm::rotate(Rotation, glm::radians(angle), axis);
	}

	void Scale(float x, float y, float z) {
		ScaleValue.x += x;
		ScaleValue.y += y;
		ScaleValue.z += z;
	}
};