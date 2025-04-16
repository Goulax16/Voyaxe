#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <stdexcept>
#include <memory>
#include <mutex>

#include "NodeTree.hpp"
#include "IRenderable.hpp"

class Scene {
private:
   mutable std::mutex nodeMutex;
   std::string sceneName;

   NodeTree& nodeTree;

public:
   explicit Scene(const std::string& name) noexcept :
       nodeTree(NodeTree::GetInstance()),
       sceneName(name) {
       std::cout << "Scene created with name: " << sceneName << std::endl;
   }

   Scene(Scene&& other) = delete;
   Scene& operator=(Scene&& other) = delete;

   ~Scene() {
       std::cout << "Scene destroyed: " << sceneName << std::endl;
       clear();
   }

   void Init() {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Initializing Scene: " << sceneName << std::endl;
       for (const auto& [name, node] : nodeTree.GetAllNodesMap()) {
           if (node) node->Init();
       }
   }

   void Update(float dt) {
       std::lock_guard<std::mutex> lock(nodeMutex);
       for (const auto& [name, node] : nodeTree.GetAllNodesMap()) {
           if (node) node->Update(dt);
       }
   }

   void Render() const {
       std::lock_guard<std::mutex> lock(nodeMutex);
       for (const auto& [name, node] : nodeTree.GetAllNodesMap()) {
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
          // Use the NodeTree's AddNode method instead of directly accessing the map  
          nodeTree.AddNode(nodeName, std::move(node));  
          std::cout << "Node added successfully: " << nodeName << std::endl;  

          nodeTree.GetNode(nodeName)->Init();  
          std::cout << "Node initialized: " << nodeName << std::endl;  
  
          std::cout << "Node count incremented: " << nodeTree.GetNodeCount() << std::endl;  
      }  
      catch (const std::exception& e) {  
          std::cerr << "Exception in Scene::AddNode: " << e.what() << std::endl;  
          throw;  
      }  
   }


   void RemoveNode(std::string_view name) {
        std::lock_guard<std::mutex> lock(nodeMutex);
        nodeTree.RemoveNode(std::string(name));
   }

   std::shared_ptr<Node> GetNode(std::string_view name) const {
       std::lock_guard<std::mutex> lock(nodeMutex);
       return nodeTree.GetNode(std::string(name));
   }

   bool NodeExists(std::string_view name) const noexcept {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Checking if node exists: " << name << std::endl;
       bool exists = nodeTree.GetAllNodesMap().find(std::string(name)) != nodeTree.GetAllNodesMap().end();
       std::cout << "Node existence result for " << name << ": " << (exists ? "Exists" : "Does not exist") << std::endl;
       return exists;
   }

   std::vector<std::shared_ptr<Node>> GetNodes() const {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Locked mutex in AddNode" << std::endl;
       std::cout << "Retrieving all nodes from Scene: " << sceneName << std::endl;
       std::vector<std::shared_ptr<Node>> nodeList;
       nodeList.reserve(nodeTree.GetAllNodesMap().size());
       for (const auto& [name, node] : nodeTree.GetAllNodesMap()) {
           nodeList.emplace_back(node);
       }
       return nodeList;
   }

   void clear() noexcept {
       std::lock_guard<std::mutex> lock(nodeMutex);
       std::cout << "Locked mutex in AddNode" << std::endl;
       std::cout << "Clearing all nodes from Scene: " << sceneName << std::endl;
       for (auto& [name, node] : nodeTree.GetAllNodesMap()) {
           if (node) node->Destroy();
       }
       nodeTree.GetAllNodes().clear();
       nodeTree.SetNodeCount(0);
   }

   size_t size() const noexcept {
       std::cout << "Scene size requested: " << nodeTree.GetNodeCount() << " nodes in Scene: " << sceneName << std::endl;
       return nodeTree.GetNodeCount();
   }

   void reserve(size_t count) {
       std::lock_guard<std::mutex> lock(nodeMutex);
       nodeTree.GetAllNodes().reserve(count);
       std::cout << "Reserved capacity for " << count << " nodes in Scene: " << sceneName << std::endl;
   }
};
