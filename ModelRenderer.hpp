#pragma once

#include "Node3D.hpp"
#include "Model.h"
#include "IRenderable.hpp"
#include "Scene.hpp"
#include "Shader.h"
#include "Camera.h"

class ModelRenderer : public Node3D, public IRenderable
{
private:
    std::unique_ptr<Model> model;
    std::unique_ptr<Shader> shader;
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
        : shader(std::make_unique<Shader>("vert.glsl", "frag.glsl")),
        cameras(),
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

        shader->Activate();

        // Set Light Properties
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
                    glm::value_ptr(cam->Position));

                model->Draw(*shader, *cam, transform.GetModelMatrix());
            }
        }
    }
    
    // Métodos para configurar la luz
    void SetLightType(int type) { lightType = type; }
    void SetLightPosition(const glm::vec3& position) { lightPos = position; }
    void SetLightDirection(const glm::vec3& direction) { lightDirection = direction; }
    void SetLightColor(const glm::vec4& color) { lightColor = color; }
    void SetSpotLightCones(float inner, float outer) { innerCone = inner; outerCone = outer; }

    void Destroy() override {
        isActive = false;
        Node::Destroy();
    }

    ModelRenderer(const ModelRenderer&) = delete;
    ModelRenderer& operator=(const ModelRenderer&) = delete;

    ModelRenderer(ModelRenderer&&) noexcept = default;
    ModelRenderer& operator=(ModelRenderer&&) noexcept = default;
};