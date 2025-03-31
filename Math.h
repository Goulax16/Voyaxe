#pragma once

#include <cmath>
#include <glm/glm.hpp> // Opcional: Usar GLM para matemáticas

// Estructura para un quaternion
struct Quaternion {
    float x, y, z, w;
};

// Normaliza un quaternion
Quaternion normalizeQuat(Quaternion q) {
    float mag = sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    return { q.x / mag, q.y / mag, q.z / mag, q.w / mag };
}

// Crea un quaternion a partir de ángulos de Euler (en radianes)
Quaternion eulerToQuaternion(float pitch, float yaw, float roll) {
    float cy = cos(yaw * 0.5f);
    float sy = sin(yaw * 0.5f);
    float cp = cos(pitch * 0.5f);
    float sp = sin(pitch * 0.5f);
    float cr = cos(roll * 0.5f);
    float sr = sin(roll * 0.5f);

    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return normalizeQuat(q); // Normalizar para evitar errores
}