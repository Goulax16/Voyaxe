#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glad/glad.h>

using vSTR = std::string;
using vCSTR = const char*;
using vInt = int;
using vFLOAT = float;
using vUINT = unsigned int;
using vBool = bool;
using vSIZEIPTR = GLsizeiptr;

using vVec2 = glm::vec2;
using vVec3 = glm::vec3;
using vVec4 = glm::vec4;
using vMat4 = glm::mat4;

class Utils {
public:
    static std::string loadResource(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};