#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
        ScaleValue *= scale;
    }

    void Translate(float x, float y, float z) {
        Position += glm::vec3(x, y, z);
    }

    void Rotate(float angle, float rx, float ry, float rz) {
        Rotation = glm::rotate(Rotation, glm::radians(angle), glm::vec3(rx, ry, rz));
    }

    void Scale(float x, float y, float z) {
        ScaleValue *= glm::vec3(x, y, z);
    }

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, Position);
        model = model * glm::mat4_cast(Rotation);
        model = glm::scale(model, ScaleValue);

        return model;
    }
};