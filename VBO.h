#pragma once

#include <glad/glad.h>
#include<vector>

#include "util.h"

struct Vertex
{
	vVec3 Pos;
	vVec3 Normal;
	vVec3 Color;
	vVec2 texUV;
};

class VBO
{
public:
	vUINT id;

	VBO(vVEC<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};

