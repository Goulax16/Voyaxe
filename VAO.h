#pragma once

#include <glad/glad.h>

#include "util.h"
#include "VBO.h"

class VAO
{
public:
	vUINT id;

	VAO();

	void LinkAttrib(VBO& vbo, vUINT layout, vUINT numComponents, GLenum type, vSIZEIPTR stride, void* offset);

	void Bind();
	void Unbind();
	void Delete();
};

