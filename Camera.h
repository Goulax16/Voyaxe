#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Shader.h"

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 orientation; // Direction vector
    glm::vec3 up;          // Up vector
    glm::quat rotation;    // Quaternion for smooth rotations

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    bool firstClick = true;

    int width, height;

    float speed = 0.1f;
    float sensitivity = 0.1f;

    void updateViewMatrix();
    void updateProjectionMatrix(float FOVdeg, float nearPlane, float farPlane);

public:
    Camera(int width, int height, glm::vec3 position);

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(Shader& shader, const char* uniform);

    void KeyboardInputs(GLFWwindow* window);
    void MouseInputs(GLFWwindow* window);

    void updateDimensions(int newWidth, int newHeight);

    glm::mat4 GetViewMatrix() const { return viewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return projectionMatrix; }

    glm::vec3 GetPosition() const { return position; }
    void SetPosition(const glm::vec3& newPosition) { position = newPosition; }

    glm::vec3 GetOrientation() const { return orientation; }
    void SetOrientation(const glm::vec3& newOrientation) { orientation = glm::normalize(newOrientation); }

    float GetSpeed() const { return speed; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }

    float GetSensitivity() const { return sensitivity; }
    void SetSensitivity(float newSensitivity) { sensitivity = newSensitivity; }
};