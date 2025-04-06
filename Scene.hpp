#pragma once

#include <unordered_map>

#include "IRenderable.hpp"
#include "Node.h"
#include "SceneManager.h"

class SceneManager;

class Scene
{
private:
	std::unordered_map<Node*, std::string> nodes;
	std::string sceneName;

	SceneManager* sceneManager;

public:

	Scene(std::string name) : sceneName(name) {}

	void SetSceneManager(SceneManager* manager) { sceneManager = manager; }
	SceneManager* GetSceneManager() const { return sceneManager; }

	void Init() {
		for (auto& pair : nodes) {
			pair.first->Init();
		}
	}

	void Update(float dt) {
		for (auto& pair : nodes) {
			pair.first->Update(dt);
		}
	}

	void Render() {
		for (auto& pair : nodes) {
			if (auto renderable = dynamic_cast<IRenderable*>(pair.first)) {
				renderable->Render();
			}
		}
	}

	void AddNode(Node* node) {
		nodes[node] = node->name;
	}

	void RemoveNode(Node* node) {
		nodes.erase(node);
	}

	Node* GetNode(std::string name) {
		for (auto& pair : nodes) {
			if (pair.first->name == name) {
				return pair.first;
			}
		}
		throw std::runtime_error("Cannot find gameObject");
	}
	std::vector<Node*> GetNodes() {
		std::vector<Node*> nodeList;
		for (const auto& pair : nodes) {
			nodeList.push_back(pair.first);
		}
		return nodeList;
	}
};