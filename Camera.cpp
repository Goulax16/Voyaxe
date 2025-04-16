#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
    : position(position), orientation(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
    width(width), height(height), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
{
    updateViewMatrix();
    updateProjectionMatrix(45.0f, 0.1f, 100.0f);
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    updateViewMatrix();
    updateProjectionMatrix(FOVdeg, nearPlane, farPlane);
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix));
}

void Camera::KeyboardInputs(GLFWwindow* window, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += (speed * orientation) * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= (speed * orientation) * dt;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= (glm::normalize(glm::cross(orientation, up)) * speed) * dt;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += (glm::normalize(glm::cross(orientation, up)) * speed) * dt;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += (up * speed) * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        position -= (up * speed) * dt;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = 0.4f;
    else
        speed = 0.1f;
}

void Camera::MouseInputs(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick)
        {
            glfwSetCursorPos(window, width / 2.0, height / 2.0);
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float deltaX = sensitivity * static_cast<float>(mouseX - width / 2.0);
        float deltaY = sensitivity * static_cast<float>(mouseY - height / 2.0);

        // Rotate using quaternions
        glm::quat pitch = glm::angleAxis(glm::radians(-deltaY), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat yaw = glm::angleAxis(glm::radians(-deltaX), glm::vec3(0.0f, 1.0f, 0.0f));

        rotation = glm::normalize(yaw * rotation * pitch);

        // Update orientation based on quaternion
        orientation = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, -1.0f));

        glfwSetCursorPos(window, width / 2.0, height / 2.0);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}

void Camera::updateViewMatrix()
{
    viewMatrix = glm::lookAt(position, position + orientation, up);
}

void Camera::updateProjectionMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    projectionMatrix = glm::perspective(glm::radians(FOVdeg), static_cast<float>(width) / height, nearPlane, farPlane);
}

void Camera::updateDimensions(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;

    updateProjectionMatrix(45.0f, 0.1f, 100.0f);
}