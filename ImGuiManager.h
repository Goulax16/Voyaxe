#pragma once

#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Node.h"
#include <vector>

class ImGuiManager {
public:
    ImGuiManager();
    ~ImGuiManager();

    void Initialize(GLFWwindow* window);
    void NewFrame();
    void Render();
    void Cleanup();

    void RenderMainMenuBar(bool& vSync, bool& vsyncChanged);
    void RenderMetrics(const std::shared_ptr<Camera>& camera);
    void RenderNodeList(const std::vector<std::weak_ptr<Node>>& nodeList);

private:
    void RenderNodeTree(Node* node);
    bool m_initialized = false;
};