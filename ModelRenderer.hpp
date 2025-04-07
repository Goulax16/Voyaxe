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
	Model* model;
	Shader* shader;
    std::vector<Camera*> cameras;
    bool isActive;

public:
    ModelRenderer(const char* modelPath, std::vector<Camera*>& cameras)
        : shader(new Shader("vert.glsl", "frag.glsl")), cameras(cameras), Node3D(), isActive(true) {
        model = new Model(modelPath);
		this->name = "ModelRenderer";
	}

    ~ModelRenderer() { Destroy(); }

    void Update(float deltaTime) override {
        if (!isActive || !model || cameras.empty()) return;
        Node3D::Update(deltaTime);
    }

    void Render() override {
        if (!isActive || !model) return;

        shader->Activate();
        for (Camera* cam : cameras) {
            glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE,
                glm::value_ptr(cam->GetViewMatrix()));
            glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE,
                glm::value_ptr(cam->GetProjectionMatrix()));

            glUniform3fv(glGetUniformLocation(shader->ID, "camPos"), 1,
                glm::value_ptr(cam->Position));

            model->Draw(*shader, *cam, transform.GetModelMatrix());
        }
    }

    void Destroy() override {
        if (!isActive) return;

        isActive = false;

        if (model) {
            delete model;
            model = nullptr;
        }

        if (shader) {
            delete shader;
            shader = nullptr;
        }

        Node::Destroy();
    }
};