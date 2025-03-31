#pragma once

#include <glad/glad.h>

#include "util.h"

class VBO
{
public:
	vUINT id;

	VBO(vFLOAT* vertices, vSIZEIPTR size);

	void Bind();
	void Unbind();
	void Delete();
};

