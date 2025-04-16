#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3 crntPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 scale; // Nuevo uniform para controlar la escala con un vec3

void main()
{
    // Aplicar la escala directamente a las coordenadas del vértice
    vec3 scaledPos = aPos * scale;

    // Transform position to world space
    crntPos = vec3(model * vec4(scaledPos, 1.0f));
    
    // Transform normal using normal matrix (transpose of inverse of model matrix)
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;
    
    // Pass through other attributes
    color = aColor;
    texCoord = aTex;
    
    // Final position in clip space
    gl_Position = projection * view * model * vec4(scaledPos, 1.0);
}