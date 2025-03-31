#pragma once

#include <glad/glad.h>

#include "util.h"

class EBO
{
public:
	vUINT id;

	EBO(vUINT* indices, vSIZEIPTR size);

	void Bind();
	void Unbind();
	void Delete();
};

