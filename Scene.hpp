#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

#include "IRenderable.hpp"
#include "Node.h"

class Scene 
{
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
        if (!node) {
            throw std::invalid_argument("Cannot add a null node.");
        }

        std::string nodeName = node->name;
        int copyIndex = 1;

        // Check if the node already exists and handle duplicates
        while (NodeExists(nodeName)) {
            nodeName = node->name + "Cpy" + std::to_string(copyIndex++);
        }

        nodes[nodeName] = node;
        node->Init();
    }

    void RemoveNode(const std::string& name) {
        auto it = nodes.find(name);
        if (it != nodes.end()) {
            nodes.erase(it);
        }
    }

    std::shared_ptr<Node> GetNode(const std::string& name) const {
        auto it = nodes.find(name);
        if (it != nodes.end()) {
            return it->second;
        }
        throw std::runtime_error("Cannot find node with name: " + name);
    }

    bool NodeExists(const std::string& name) const {
        return nodes.find(name) != nodes.end();
    }

    std::vector<std::shared_ptr<Node>> GetNodes() const {
        std::vector<std::shared_ptr<Node>> nodeList;
        for (const auto& [name, node] : nodes) {
            nodeList.push_back(node);
        }
        return nodeList;
    }
};