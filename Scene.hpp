#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

#include "IRenderable.hpp"
#include "Node.h"

class Scene {
private:
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    std::string sceneName;

public:
    explicit Scene(const std::string& name) : sceneName(name) {}

    void Init() {
        for (const auto& [name, node] : nodes) {
            node->Init();
        }
    }

    void Update(float dt) {
        for (const auto& [name, node] : nodes) {
            node->Update(dt);
        }
    }

    void Render() {
        for (const auto& [name, node] : nodes) {
            if (auto renderable = std::dynamic_pointer_cast<IRenderable>(node)) {
                renderable->Render();
            }
        }
    }

    void AddNode(std::shared_ptr<Node> node) {
        if (node) {
            nodes[node->name] = node;
            node->Init();
        }
    }

    void RemoveNode(const std::string& name) {
        nodes.erase(name);
    }

    std::shared_ptr<Node> GetNode(const std::string& name) const {
        auto it = nodes.find(name);
        if (it != nodes.end()) {
            return it->second;
        }
        throw std::runtime_error("Cannot find node with name: " + name);
    }

    std::vector<std::shared_ptr<Node>> GetNodes() const {
        std::vector<std::shared_ptr<Node>> nodeList;
        for (const auto& [name, node] : nodes) {
            nodeList.push_back(node);
        }
        return nodeList;
    }
};