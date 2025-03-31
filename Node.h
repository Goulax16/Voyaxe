#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "util.h"

class Node
{
protected:
	Node* parent = nullptr;
	std::vector<std::unique_ptr<Node>> children;

	vSTR name;

public:
	Node() : name("Node") {}
	virtual ~Node() { delete this; }

	void AddChild(std::unique_ptr<Node> node) {
		node->parent = this;
		children.push_back(std::move(node));
	}

	void RemoveChild(const vSTR& name) {
		auto it = std::find_if(children.begin(), children.end(),
			[&name, this](const std::unique_ptr<Node>& child) {
				return child.get() == this->GetChild(name);
			});

		if (it != children.end()) {
			children.erase(it);
		}
	}

	const Node* GetChild(const vSTR& name) const {
		auto it = std::find_if(children.begin(), children.end(),
			[&name](const std::unique_ptr<Node>& child) {
				return child->name == name;
			});

		return it != children.end() ? it->get() : nullptr;
	}

	virtual void Init() {
		for (auto& child : children) {
			child->Init();
		}
	}

	virtual void Update(float deltaTime) {
		for (auto& child : children) {
			child->Update(deltaTime);
		}
	}
};