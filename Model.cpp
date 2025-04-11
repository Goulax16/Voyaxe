#include "Model.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Helper function to read file contents
std::string get_file_contents(const char* file) {
    std::ifstream ifs(file, std::ios::in | std::ios::binary | std::ios::ate);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + std::string(file));
    }
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::string buffer(size, '\0');
    if (!ifs.read(&buffer[0], size)) {
        throw std::runtime_error("Failed to read file: " + std::string(file));
    }
    return buffer;
}

Model::Model(const char* file) : file(file) {
    try {
        // Parse JSON data
        std::string text = get_file_contents(file);
        JSON = json::parse(text);

        // Load binary data
        data = getData();

        // Traverse nodes starting from the root
        traverseNode(0);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error loading model: " + std::string(e.what()));
    }
}

Model::~Model() {
    for (auto& mesh : meshes) {
        mesh.Cleanup();
    }
}

void Model::Draw(Shader& shader, Camera& camera, const glm::mat4& parentModelMatrix) {
    shader.Activate();
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"),
        camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), 1.0f, 1.0f, 1.0f);

    for (unsigned int i = 0; i < meshes.size(); i++) {
        glm::mat4 combinedMatrix = parentModelMatrix * matricesMeshes[i];
        meshes[i].Draw(shader, camera, combinedMatrix);
    }
}

void Model::loadMesh(unsigned int indMesh) {
    unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

    std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);

    std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
    std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);

    std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
    std::vector<Texture> textures = getTextures();

    meshes.emplace_back(vertices, indices, textures);
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix) {
    json node = JSON["nodes"][nextNode];

    glm::vec3 translation = glm::vec3(0.0f);
    if (node.contains("translation")) {
        translation = glm::vec3(node["translation"][0], node["translation"][1], node["translation"][2]);
    }

    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.contains("rotation")) {
        rotation = glm::quat(node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2]);
    }

    glm::vec3 scale = glm::vec3(1.0f);
    if (node.contains("scale")) {
        scale = glm::vec3(node["scale"][0], node["scale"][1], node["scale"][2]);
    }

    glm::mat4 matNode = glm::mat4(1.0f);
    if (node.contains("matrix")) {
        matNode = glm::make_mat4(node["matrix"].data());
    }

    glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 rot = glm::mat4_cast(rotation);
    glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);

    glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

    if (node.contains("mesh")) {
        translationsMeshes.push_back(translation);
        rotationsMeshes.push_back(rotation);
        scalesMeshes.push_back(scale);
        matricesMeshes.push_back(matNextNode);
        loadMesh(node["mesh"]);
    }

    if (node.contains("children")) {
        for (unsigned int child : node["children"]) {
            traverseNode(child, matNextNode);
        }
    }
}

std::vector<unsigned char> Model::getData() {
    std::string uri = JSON["buffers"][0]["uri"];
    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    std::string bytesText = get_file_contents((fileDirectory + uri).c_str());
    return std::vector<unsigned char>(bytesText.begin(), bytesText.end());
}

std::vector<float> Model::getFloats(const json& accessor) {
    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    unsigned int numPerVert = 0;
    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else throw std::invalid_argument("Invalid type: " + type);

    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;

    std::vector<float> floatVec;
    floatVec.reserve(count * numPerVert);

    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i += 4) {
        unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<GLuint> Model::getIndices(const json& accessor) {
    unsigned int buffViewInd = accessor.value("bufferView", 0);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];

    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];
    unsigned int beginningOfData = byteOffset + accByteOffset;

    std::vector<GLuint> indices;
    indices.reserve(count);

    if (componentType == 5125) { // Unsigned int
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 4; i += 4) {
            unsigned char bytes[] = { data[i], data[i + 1], data[i + 2], data[i + 3] };
            GLuint value;
            std::memcpy(&value, bytes, sizeof(GLuint));
            indices.push_back(value);
        }
    }
    else if (componentType == 5123) { // Unsigned short
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i += 2) {
            unsigned char bytes[] = { data[i], data[i + 1] };
            GLushort value;
            std::memcpy(&value, bytes, sizeof(GLushort));
            indices.push_back(static_cast<GLuint>(value));
        }
    }
    else if (componentType == 5122) { // Short
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i += 2) {
            unsigned char bytes[] = { data[i], data[i + 1] };
            GLshort value;
            std::memcpy(&value, bytes, sizeof(GLshort));
            indices.push_back(static_cast<GLuint>(value));
        }
    }

    return indices;
}

std::vector<Texture> Model::getTextures() {
    std::vector<Texture> textures;
    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    for (const auto& image : JSON["images"]) {
        std::string texPath = image["uri"];

        if (loadedTextures.find(texPath) != loadedTextures.end()) {
            textures.push_back(loadedTextures[texPath]);
            continue;
        }

        Texture texture;
        if (texPath.find("baseColor") != std::string::npos) {
            texture = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTextures.size());
        }
        else if (texPath.find("metallicRoughness") != std::string::npos) {
            texture = Texture((fileDirectory + texPath).c_str(), "specular", loadedTextures.size());
        }
        else {
            continue;
        }

        textures.push_back(texture);
        loadedTextures[texPath] = texture;
    }

    return textures;
}

std::vector<Vertex> Model::assembleVertices(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::vec2>& texUVs) {

    std::vector<Vertex> vertices;
    vertices.reserve(positions.size());

    for (size_t i = 0; i < positions.size(); ++i) {
        vertices.emplace_back(Vertex{
            positions[i],
            normals[i],
            glm::vec3(1.0f),
            texUVs[i]
            });
    }

    return vertices;
}

std::vector<glm::vec2> Model::groupFloatsVec2(const std::vector<float>& floatVec) {
    std::vector<glm::vec2> vectors;
    vectors.reserve(floatVec.size() / 2);

    for (size_t i = 0; i < floatVec.size(); i += 2) {
        vectors.emplace_back(glm::vec2(floatVec[i], floatVec[i + 1]));
    }

    return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(const std::vector<float>& floatVec) {
    std::vector<glm::vec3> vectors;
    vectors.reserve(floatVec.size() / 3);

    for (size_t i = 0; i < floatVec.size(); i += 3) {
        vectors.emplace_back(glm::vec3(floatVec[i], floatVec[i + 1], floatVec[i + 2]));
    }

    return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(const std::vector<float>& floatVec) {
    std::vector<glm::vec4> vectors;
    vectors.reserve(floatVec.size() / 4);

    for (size_t i = 0; i < floatVec.size(); i += 4) {
        vectors.emplace_back(glm::vec4(floatVec[i], floatVec[i + 1], floatVec[i + 2], floatVec[i + 3]));
    }

    return vectors;
}