#include "Window.h"
#include "Camera.h"
#include <iostream>
#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

using namespace ImGui;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    if (width > 0 && height > 0) glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title) {
}

Window::~Window() { Cleanup(); }

Window::Window(Window&& other) noexcept
    : m_window(std::exchange(other.m_window, nullptr)),
    m_width(other.m_width),
    m_height(other.m_height),
    m_title(std::move(other.m_title)),
    m_camera(std::move(other.m_camera)),
    m_imguiInitialized(other.m_imguiInitialized) {
}

Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        Cleanup();
        m_window = std::exchange(other.m_window, nullptr);
        m_width = other.m_width;
        m_height = other.m_height;
        m_title = std::move(other.m_title);
        m_camera = std::move(other.m_camera);
        m_imguiInitialized = other.m_imguiInitialized;
    }
    return *this;
}

bool Window::Initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    m_camera = std::make_shared<Camera>(m_width, m_height, glm::vec3(0.0f, 0.0f, 2.0f));
    InitializeImGui();
    return true;
}

void Window::Run() {
    if (!m_window) throw std::runtime_error("Window not initialized");
    while (!glfwWindowShouldClose(m_window)) Update();
}

void Window::Update() {
    ProcessInput();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.10f, 0.23f, 0.32f, 1.0f);
    for (auto& callback : m_renderCallbacks) callback();
    CaptureWindowToTexture();

    if (m_imguiInitialized) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        RenderImGui();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::CaptureWindowToTexture() {
    if (!m_captureFramebuffer) {
        glGenFramebuffers(1, &m_captureFramebuffer);
        glGenTextures(1, &m_captureTexture);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_captureFramebuffer);
    glBindTexture(GL_TEXTURE_2D, m_captureTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_captureTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Capture framebuffer is not complete!\n";

    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    m_imguiInitialized = true;
}

void Window::AddRenderCallback(RenderingCallback callback) { m_renderCallbacks.push_back(callback); }

void Window::RemoveAllRenderCallbacks() { m_renderCallbacks.clear(); }

void Window::ProcessInput() {
    if (!m_camera) return;

    m_camera->KeyboardInputs(m_window);

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) m_camera->MouseInputs(m_window);

    m_camera->updateMatrix(45.0f, 0.01f, 100.0f);
}

void Window::RenderImGui() {
    if (!m_camera) return;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                Cleanup();
                glfwSetWindowShouldClose(m_window, true);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Metrics");
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("FPS: %.1f", io.Framerate);
    const auto& position = m_camera->GetPosition();
    const auto& orientation = m_camera->GetOrientation();
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    ImGui::Text("Orientation: (%.2f, %.2f, %.2f)", orientation.x, orientation.y, orientation.z);
    ImGui::End();

    ImGui::Begin("Node List");
    if (ImGui::TreeNode("Nodes")) {
        for (auto& weakNode : m_nodeList) {
            if (auto node = weakNode.lock()) { // Check if the node still exists
                RenderNodeTree(node.get());
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
}

void Window::RenderNodeTree(Node* node) {
    if (ImGui::TreeNode(node->name.c_str())) {
        if (node->HasChildren()) {
            for (const auto& child : node->GetChildren()) {
                RenderNodeTree(child.get());
            }
        }
        ImGui::TreePop();
    }
}

void Window::Cleanup() {
    if (m_captureFramebuffer) glDeleteFramebuffers(1, &m_captureFramebuffer);
    if (m_captureTexture) glDeleteTextures(1, &m_captureTexture);
    if (m_imguiInitialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    if (m_window) glfwDestroyWindow(m_window);
    glfwTerminate();
}