#pragma once

#include "Mesh.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <json/json.hpp>
#include "Shader.h"
#include "Camera.h"

using json = nlohmann::json;

class Model
{
private:
    std::vector<unsigned char> data;
    json JSON;
    const char* file;

    std::vector<Mesh> meshes;
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    std::vector<Texture> loadedTex;
    std::vector<std::string> loadedTexName;

public:
    Model(const char* file);
    ~Model();

    void Draw(Shader& shader, Camera& camera, const glm::mat4& parentModelMatrix = glm::mat4(1.0f));

private:
    void loadMesh(unsigned int indMesh);
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));
    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture> getTextures();
    std::vector<Vertex> assembleVertices(
        std::vector<glm::vec3> positions,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texUVs);

    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};