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

	void Init() override {
		std::cout << "X: " << transform.Position.x 
			<< "\nY: " << transform.Position.y 
			<< "\nZ: " << transform.Position.z << std::endl;
	}
};