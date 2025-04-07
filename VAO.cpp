#include "VAO.h"

vao::vao() {
	glGenVertexArrays(1, &id);
}

void vao::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void vao::Bind()
{
	glBindVertexArray(id);
}

void vao::Unbind()
{
	glBindVertexArray(0);
}

void vao::Delete()
{
	glDeleteVertexArrays(1, &id);
}