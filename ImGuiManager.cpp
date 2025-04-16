#include "ImGuiManager.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

ImGuiManager::ImGuiManager() {}

ImGuiManager::~ImGuiManager() {
    Cleanup();
}

void ImGuiManager::Initialize(GLFWwindow* window) {
    if (m_initialized) return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    m_initialized = true;
}

void ImGuiManager::NewFrame() {
    if (!m_initialized) return;
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render() {
    if (!m_initialized) return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Cleanup() {
    if (!m_initialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    m_initialized = false;
}

void ImGuiManager::RenderMainMenuBar(bool& vSync, bool& vsyncChanged) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Actions")) {
            if (ImGui::MenuItem("VSync", nullptr, vSync)) {
                vSync = !vSync;
                vsyncChanged = true;
            }
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void ImGuiManager::RenderMetrics(const std::shared_ptr<Camera>& camera) {
    if (!camera) return;

    ImGui::Begin("Metrics");
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %.1f", io.Framerate);
    const auto& position = camera->GetPosition();
    const auto& orientation = camera->GetOrientation();
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    ImGui::Text("Orientation: (%.2f, %.2f, %.2f)", orientation.x, orientation.y, orientation.z);
    ImGui::End();
}

void ImGuiManager::RenderNodeList(const std::vector<std::weak_ptr<Node>>& nodeList) {
    ImGui::Begin("Node List");
    if (ImGui::TreeNode("Nodes")) {
        for (auto& weakNode : nodeList) {
            if (auto node = weakNode.lock()) {
                RenderNodeTree(node.get());
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

void ImGuiManager::RenderNodeTree(Node* node) {
    if (!node) return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    bool nodeOpen = ImGui::TreeNodeEx(node->GetName().c_str(), flags);

    if (nodeOpen) {
        for (const auto& child : node->GetChildren()) {
            RenderNodeTree(child.get());
        }
        ImGui::TreePop();
    }
}