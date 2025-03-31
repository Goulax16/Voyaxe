#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "util.h"

class Node : public std::enable_shared_from_this<Node>
{
public:
	using Ptr = std::shared_ptr<Node>;

	Node(const std::string& name = "Node");
	virtual ~Node();

	virtual void Update(float delta);
	virtual void Ready() {}

	void AddChild(Ptr child);
	void RemoveChild(Ptr child);
	Ptr FindChild(const std::string& name, bool recursive = true) const;
	std::vector<Ptr> GetChildren() const;
	Node* GetParent() const;

	void SetName(const std::string& name);
	std::string GetName() const;
	std::string GetPath() const;

	virtual glm::mat4 GetGlobalTransform() const { return glm::mat4(1.0f); }

protected:
	std::string name_;
	Node* parent_ = nullptr;
	std::vector<Ptr> children_;
};