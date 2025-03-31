#pragma once

#include "Node3D.hpp"

class MeshRenderer : public Node3D
{
public:
	MeshRenderer() : Node3D() {
		this->name = "MeshRenderer";
	}
};