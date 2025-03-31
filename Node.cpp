#include "Node.h"

Node::Node(const std::string& name) : name_(name) {}

Node::~Node() {
    for (auto& child : children_) {
        child->parent_ = nullptr;
    }
}

void Node::AddChild(Ptr child) {
    if (!child) return;

    if (child->parent_) {
        child->parent_->RemoveChild(child);
    }

    children_.push_back(child);
    child->parent_ = this;
    child->Ready();
}

void Node::RemoveChild(Ptr child) {
    auto it = std::remove_if(children_.begin(), children_.end(),
        [&](const Ptr& ptr) { return ptr.get() == child.get(); });

    if (it != children_.end()) {
        (*it)->parent_ = nullptr;
        children_.erase(it, children_.end());
    }
}

Node::Ptr Node::FindChild(const std::string& name, bool recursive) const {
    for (const auto& child : children_) {
        if (child->name_ == name) {
            return child;
        }

        if (recursive) {
            auto found = child->FindChild(name, true);
            if (found) return found;
        }
    }
    return nullptr;
}

void Node::Update(float delta) {
    for (const auto& child : children_) {
        child->Update(delta);
    }
}

std::string Node::GetPath() const {
    if (!parent_) return "/" + name_;
    return parent_->GetPath() + "/" + name_;
}