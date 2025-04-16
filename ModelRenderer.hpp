#pragma once

#include "Node3D.hpp"
#include "Model.h"
#include "IRenderable.hpp"
#include "Scene.hpp"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include <memory>

class ModelRenderer : public Node3D, public IRenderable
{
private:
    std::shared_ptr<Material> material;
    std::unique_ptr<Model> model;
    std::vector<std::weak_ptr<Camera>> cameras;
    bool isActive;

    // Light Properties
    int lightType;  // 0 = point, 1 = directional, 2 = spot
    glm::vec3 lightPos;
    glm::vec3 lightDirection;
    glm::vec4 lightColor;
    float outerCone;
    float innerCone;

public:
    ModelRenderer(const char* modelPath, const std::vector<std::shared_ptr<Camera>>& cameras)
        : cameras(),
        Node3D(),
        isActive(true),
        lightType(1), // Default: Directional Light
        lightPos(5.0f, 5.0f, 5.0f),
        lightDirection(-0.2f, -1.0f, -0.3f),
        lightColor(1.0f, 1.0f, 1.0f, 1.0f),
        outerCone(0.90f),
        innerCone(0.95f)
    {
        model = std::make_unique<Model>(modelPath);
        auto shader = std::make_shared<Shader>("vert.glsl", "frag.glsl");
        material = std::make_shared<Material>(shader);
        this->name = "ModelRenderer";

        // Store weak pointers to cameras to avoid circular references
        for (const auto& cam : cameras) {
            this->cameras.emplace_back(cam);
        }
    }

    ~ModelRenderer() override = default;

    void Update(float deltaTime) override {
        if (!isActive || !model || cameras.empty()) return;
        Node3D::Update(deltaTime);
    }

    void Render() override {
        if (!isActive || !model || cameras.empty()) return;

        material->Use();

        // Set Light Properties
        auto shader = material->shader;
        glUniform1i(glGetUniformLocation(shader->ID, "lightType"), lightType);
        glUniform3fv(glGetUniformLocation(shader->ID, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform4fv(glGetUniformLocation(shader->ID, "lightColor"), 1, glm::value_ptr(lightColor));

        if (lightType == 1 || lightType == 2) { // Directional or Spot
            glUniform3fv(glGetUniformLocation(shader->ID, "lightDirection"), 1, glm::value_ptr(lightDirection));
        }

        if (lightType == 2) { // Spot light
            glUniform1f(glGetUniformLocation(shader->ID, "outerCone"), outerCone);
            glUniform1f(glGetUniformLocation(shader->ID, "innerCone"), innerCone);
        }

        // Iterate over cameras using weak pointers
        for (auto& weakCam : cameras) {
            if (auto cam = weakCam.lock()) { // Check if the camera still exists
                glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE,
                    glm::value_ptr(cam->GetViewMatrix()));
                glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE,
                    glm::value_ptr(cam->GetProjectionMatrix()));

                glUniform3fv(glGetUniformLocation(shader->ID, "camPos"), 1,
                    glm::value_ptr(cam->GetPosition()));

                // Get the model matrix from the Transform object
                glm::mat4 modelMatrix = transform.GetModelMatrix();

                // Pass the model matrix to the shader
                glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                    glm::value_ptr(modelMatrix));

                glm::vec3 scaleVector = transform.ScaleValue;
                glUniform3fv(glGetUniformLocation(shader->ID, "scale"), 1, glm::value_ptr(scaleVector));

                // Draw the model
                model->Draw(*shader, *cam, modelMatrix);
            }
        }
    }
    
    // Métodos para configurar la luz
    void SetLightType(int type) { lightType = type; }
    void SetLightPosition(const glm::vec3& position) { lightPos = position; }
    void SetLightDirection(const glm::vec3& direction) { lightDirection = direction; }
    void SetLightColor(const glm::vec4& color) { lightColor = color; }
    void SetSpotLightCones(float inner, float outer) { innerCone = inner; outerCone = outer; }

    void ChangeColor(glm::vec3 newColor) const {
        material->tintColor = newColor;
    }

    void Destroy() override {
        isActive = false;
        Node::Destroy();
    }

    void ChangeScale(const glm::vec3& scale) {
        transform.Scale(scale.x, scale.y, scale.z);
    }

    void ChangeScale(float x, float y, float z) {
        transform.Scale(x, y, z);
    }

    void ChangePos(const glm::vec3& pos) {
        transform.Translate(pos.x, pos.y, pos.z);
    }

    void ChangePos(float x, float y, float z) {
        transform.Translate(x, y, z);
    }

    int GetLightType() { return lightType; }
    glm::vec4 GetLightColor() { return lightColor; }

    Material GetMaterial() const { return *material; }

    ModelRenderer(const ModelRenderer&) = delete;
    ModelRenderer& operator=(const ModelRenderer&) = delete;

    ModelRenderer(ModelRenderer&&) noexcept = default;
    ModelRenderer& operator=(ModelRenderer&&) noexcept = default;
};