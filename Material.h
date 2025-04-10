#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Shader.h"

class Texture;

class Material
{
public:
	Material();
	~Material();

    glm::vec3 albedo = glm::vec3(0.8f);
    float metallic = 0.0f;
    float roughness = 0.5f;
    glm::vec3 emission = glm::vec3(0.0f);
    float alpha = 1.0f;

    std::shared_ptr<Texture> albedoMap;
    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> metallicRoughnessMap;

    void SetAlbedo(const glm::vec3& color);
    void SetMetallic(float value);
    void SetRoughness(float value);
    void SetEmission(const glm::vec3& color);

    void LoadAlbedoMap(const std::string& path);
    void LoadNormalMap(const std::string& path);
    void LoadMetallicRoughnessMap(const std::string& path);

    void Bind(Shader* shader) const;
};

