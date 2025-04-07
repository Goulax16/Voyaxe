#pragma once

#include <glad/glad.h>

#include "util.h"

class EBO
{
public:
	vUINT id;

	EBO(vVEC<vUINT>& indices);

	void Bind();
	void Unbind();
	void Delete();
};

