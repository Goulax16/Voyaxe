#pragma once

#include "Mesh.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <json/json.hpp>
#include "Shader.h"
#include "Camera.h"

using json = nlohmann::json;

class Model
{
private:
    std::vector<unsigned char> data; // Binary data from the model file
    json JSON;                        // Parsed JSON data
    const char* file;                 // Path to the model file

    std::vector<Mesh> meshes;         // List of meshes in the model
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    std::unordered_map<std::string, Texture> loadedTextures; // Cache for loaded textures

    void loadMesh(unsigned int indMesh);
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));
    std::vector<unsigned char> getData();
    std::vector<float> getFloats(const json& accessor);
    std::vector<GLuint> getIndices(const json& accessor);
    std::vector<Texture> getTextures();
    std::vector<Vertex> assembleVertices(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texUVs);

    std::vector<glm::vec2> groupFloatsVec2(const std::vector<float>& floatVec);
    std::vector<glm::vec3> groupFloatsVec3(const std::vector<float>& floatVec);
    std::vector<glm::vec4> groupFloatsVec4(const std::vector<float>& floatVec);

public:
    Model(const char* file);
    ~Model();

    void Draw(Shader& shader, Camera& camera, const glm::mat4& parentModelMatrix = glm::mat4(1.0f));
};