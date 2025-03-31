#pragma once

#include "Node.h"

class Node3D : public Node
{
public:
	Node3D() : Node() {
		this->name = "Node3D";
	}

	void Init() override {
		std::cout << "I exist" << std::endl;
	}
};