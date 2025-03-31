#pragma once

#include "Node.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Spatial : public Node {
public:
    void SetPosition(const glm::vec3& position) {
        position_ = position;
        transform_dirty_ = true;
    }

    void SetRotation(const glm::quat& rotation);
    void SetScale(const glm::vec3& scale);

    glm::vec3 GetGlobalPosition() const;
    glm::mat4 GetGlobalTransform() const;

    void Update(float delta) override;

protected:
    glm::vec3 position_ = { 0, 0, 0 };
    glm::quat rotation_ = { 1, 0, 0, 0 };
    glm::vec3 scale_ = { 1, 1, 1 };
    mutable glm::mat4 global_transform_;
    mutable bool transform_dirty_ = true;
};