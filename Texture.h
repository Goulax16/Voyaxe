#pragma once

#include "util.h"
#include "Shader.h"

#include <glad/glad.h>
#include <stb_image.h>

class Texture
{
public:
	vUINT id;
	GLenum type;
	Texture(vCSTR image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, vCSTR uniform, vUINT unit);
	void Bind();
	void Unbind();
	void Delete();
};

