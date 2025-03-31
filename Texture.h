#pragma once

#include "util.h"
#include "Shader.h"

#include <glad/glad.h>
#include <stb_image.h>

class Texture
{
public:
	GLuint ID;
	vCSTR type;
	GLuint unit;

	Texture(vCSTR image, vCSTR texType, GLuint slot);

	void texUnit(Shader& shader, vCSTR uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};

