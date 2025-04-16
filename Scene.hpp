#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <stdexcept>
#include <memory>
#include <mutex>

#include "IRenderable.hpp"
#include "Node.h"

class Scene {
private:
   mutable std::mutex nodeMutex;
   std::unordered_map<std::string, std::shared_ptr<Node>> nodes;
   std::string sceneName;
   size_t nodeCount{ 0 };

public:
   explicit Scene(const std::string& name) noexcept :
       nodes(16), // Initial capacity hint
       sceneName(name) {
       std::cout << "Scene created with name: " << sceneName << std::endl;
   }

   Scene(Scene&& other) noexcept :
       nodes(std::move(other.nodes)),
       sceneName(std::move(other.sceneName)),
       nodeCount(other.nodeCount) {
       other.nodeCount = 0;
       std::cout << "Scene moved: " << sceneName << std::endl;
   }

   Scene& operator=(Scene&& other) noexcept {
       if (this != &other) {
           std::lock_guard<std::mutex> lock(nodeMutex);
           nodes = std::move(other.nodes);
           sceneName = std::move(other.sceneName);
           nodeCount = other.nodeCount;
           other.nodeCount = 0;
           std::cout << "Scene move-assigned: " << sceneName << std::endl;
       }
       return *this;
   }

   ~Scene() {
       std::cout << "Scene destroyed: " << sceneName << std::endl;
       clear();
   }

   void Init() {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Initializing Scene: " << sceneName << std::endl;
       for (const auto& [name, node] : nodes) {
           if (node) node->Init();
       }
   }

   void Update(float dt) {
       std::lock_guard<std::mutex> lock(nodeMutex);
       for (const auto& [name, node] : nodes) {
           if (node) node->Update(dt);
       }
   }

   void Render() const {
       std::lock_guard<std::mutex> lock(nodeMutex);
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

       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Locked mutex in AddNode" << std::endl;

       std::string nodeName = node->name;

       // Skip name collision checking for testing
       std::cout << "Skipping name collision checking for node: " << nodeName << std::endl;

       try {
           nodes.emplace(nodeName, std::move(node));
           std::cout << "Node emplaced successfully: " << nodeName << std::endl;

           nodes[nodeName]->Init();
           std::cout << "Node initialized: " << nodeName << std::endl;

           ++nodeCount;
           std::cout << "Node count incremented: " << nodeCount << std::endl;
       }
       catch (const std::exception& e) {
           std::cerr << "Exception in Scene::AddNode: " << e.what() << std::endl;
           throw;
       }
   }


   void RemoveNode(std::string_view name) {
       std::lock_guard<std::mutex> lock(nodeMutex);
       if (auto it = nodes.find(std::string(name)); it != nodes.end()) {
           it->second->Destroy();
           nodes.erase(it);
           --nodeCount;
           std::cout << "Node removed: " << name << " from Scene: " << sceneName << std::endl;
       }
   }

   std::shared_ptr<Node> GetNode(std::string_view name) const {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Locked mutex in AddNode" << std::endl;
       if (auto it = nodes.find(std::string(name)); it != nodes.end()) {
           std::cout << "Node retrieved: " << name << " from Scene: " << sceneName << std::endl;
           return it->second;
       }
       throw std::runtime_error("Cannot find node with name: " + std::string(name));
   }

   bool NodeExists(std::string_view name) const noexcept {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Checking if node exists: " << name << std::endl;
       bool exists = nodes.find(std::string(name)) != nodes.end();
       std::cout << "Node existence result for " << name << ": " << (exists ? "Exists" : "Does not exist") << std::endl;
       return exists;
   }

   std::vector<std::shared_ptr<Node>> GetNodes() const {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Locked mutex in AddNode" << std::endl;
       std::cout << "Retrieving all nodes from Scene: " << sceneName << std::endl;
       std::vector<std::shared_ptr<Node>> nodeList;
       nodeList.reserve(nodes.size());
       for (const auto& [name, node] : nodes) {
           nodeList.emplace_back(node);
       }
       return nodeList;
   }

   void clear() noexcept {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Locked mutex in AddNode" << std::endl;
       std::cout << "Clearing all nodes from Scene: " << sceneName << std::endl;
       for (auto& [name, node] : nodes) {
           if (node) node->Destroy();
       }
       nodes.clear();
       nodeCount = 0;
   }

   size_t size() const noexcept {
       std::cout << "Scene size requested: " << nodeCount << " nodes in Scene: " << sceneName << std::endl;
       return nodeCount;
   }

   void reserve(size_t count) {
       std::lock_guard<std::mutex> lock(nodeMutex);
       nodes.reserve(count);
       std::cout << "Reserved capacity for " << count << " nodes in Scene: " << sceneName << std::endl;
   }
};
