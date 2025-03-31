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

	void AddChild(Ptr child);
	void RemoveChild(Ptr child);
	Ptr GetChild(const vSTR& name) const;
	std::vector<Ptr> GetChildren() const;

	void SetName(const std::string& name);
	std::string GetName() const;

	virtual void Update(float delta);

protected:
	std::string name_;
	Node* parent_ = nullptr;
	std::vector<Ptr> children_;
};