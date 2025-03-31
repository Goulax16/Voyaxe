#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;

// Outputs
out vec3 color;
out vec2 texCoord;

// Uniforms
uniform mat4 camMatrix;

uniform vec4 rotationQuat;

void main()
{
    float qx = rotationQuat.x;
    float qy = rotationQuat.y;
    float qz = rotationQuat.z;
    float qw = rotationQuat.w;
    
    mat4 rotMatrix = mat4(
        1 - 2*qy*qy - 2*qz*qz,     2*qx*qy - 2*qz*qw,     2*qx*qz + 2*qy*qw,     0.0,
        2*qx*qy + 2*qz*qw,         1 - 2*qx*qx - 2*qz*qz, 2*qy*qz - 2*qx*qw,     0.0,
        2*qx*qz - 2*qy*qw,         2*qy*qz + 2*qx*qw,     1 - 2*qx*qx - 2*qy*qy, 0.0,
        0.0,                        0.0,                    0.0,                    1.0
    );
    
    // Aplicar transformaciones
    gl_Position = camMatrix * rotMatrix * vec4(aPos, 1.0);
    color = aColor;
    texCoord = aTex;
}