#include "Node.h"

Node::Node(const vSTR& name) : name_(name) {}

Node::~Node() {
	for (auto& child : children_) {
		child->parent_->RemoveChild(child);
	}
}

void Node::AddChild(Ptr child) {
    if (child->parent_) {
        child->parent_->RemoveChild(child);
    }
    children_.push_back(child);
    child->parent_ = this;
}

void Node::RemoveChild(Ptr child) {
    auto it = std::find_if(children_.begin(), children_.end(),
        [&](const Ptr& ptr) { return ptr.get() == child.get(); });

    if (it != children_.end()) {
        (*it)->parent_ = nullptr;
        children_.erase(it);
    }
}

void Node::Update(float delta) {
    for (auto& child : children_) {
        child->Update(delta);
    }
}