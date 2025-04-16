#pragma once

#include <cstddef>
#include <vector>
#include <memory>
#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<Texture>> textures;

    glm::vec3 tintColor = glm::vec3(1.0f, 1.0f, 1.0f);

    Material(std::shared_ptr<Shader> shader) : shader(shader) {}

    void AddTexture(std::shared_ptr<Texture> texture) {
        textures.push_back(texture);
    }

    void Use() {
        if (shader) {
            shader->Activate();

            glUniform3fv(glGetUniformLocation(shader->ID, "tintColor"), 1, glm::value_ptr(tintColor));
            for (size_t i = 0; i < textures.size(); i++) {
                textures[i]->Bind();
                textures[i]->texUnit(*shader, ("texture" + std::to_string(i)).c_str(), i);
            }
        }
    }
};

