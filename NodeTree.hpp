#pragma once

#include "Node.h"

class NodeTree
{
private:
    std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
    size_t nodeCount{ 0 };

    NodeTree() : nodes(16) {}

    NodeTree(const NodeTree&) = delete;
    NodeTree& operator=(const NodeTree&) = delete;
public:
    static NodeTree& GetInstance() {
        static NodeTree instance;
        return instance;
    }

    void AddNode(const std::string& name, std::shared_ptr<Node> node) {
        if (nodes.find(name) != nodes.end()) {
            throw std::runtime_error("Node with name: " + name + " already exists!");
        }
        nodes[name] = node;
        ++nodeCount;
    }

    void RemoveNode(const std::string& name) {
        auto it = nodes.find(name);
        if (it == nodes.end()) {
            throw std::runtime_error("Node with name: " + name + " doesn't exists!");
        }
        nodes.erase(it);
        --nodeCount;
    }
    
    std::shared_ptr<Node> GetNode(const std::string& name) const {
        auto it = nodes.find(name);
        if (it == nodes.end()) {
            throw std::runtime_error("Node with name: " + name + " not found!");
        }
        return it->second;
    }

    size_t GetNodeCount() const {
        return nodeCount;
    }

    std::vector<std::shared_ptr<Node>> GetAllNodes() const {
        std::vector<std::shared_ptr<Node>> result;
        result.reserve(nodes.size());
        for (const auto& [id, node] : nodes) {
            result.push_back(node);
        }
        return result;
    }

    const std::unordered_map<std::string, std::shared_ptr<Node>>& GetAllNodesMap() const {
        return nodes;
    }
};