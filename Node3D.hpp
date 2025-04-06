#pragma once

#include "Node.h"
#include "Transform.hpp"

class Node3D : public Node
{
public:
	Transform transform;

	Node3D() : Node() {
		this->name = "Node3D";
	}

	virtual void Update(float deltaTime) override {}
	virtual void Init() override {}

	virtual void Render() override {}
};