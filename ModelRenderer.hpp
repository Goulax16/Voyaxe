#pragma once

#include "Node3D.hpp"
#include "Model.h"

class ModelRenderer : public Node3D
{
private:
	Model model;
	Shader shader;
	Camera cam;

public:
	ModelRenderer(vCSTR modelPath, Shader shader, Camera cam) : shader(shader), cam(cam), model(modelPath), Node3D() {
		this->name = "MeshRenderer";
	}

	void Update(float deltaTime) override {		
		model.Draw(shader, cam);
	}
};