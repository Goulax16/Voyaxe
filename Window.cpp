#include "Window.h"
#include "Camera.h"
#include <iostream>
#include <stdexcept>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->SetViewportSize({ static_cast<float>(width), static_cast<float>(height) });
    }
}

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title) {
}

Window::~Window() {
    Cleanup();
}

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
        std::cerr << "Failed to initialize GLFW" << std::endl;
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
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    SetFramebufferSizeCallback(FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, m_width, m_height);
    glEnable(GL_DEPTH_TEST);

    m_viewportSize = { static_cast<float>(m_width), static_cast<float>(m_height) };
    SetupViewport();

    m_camera = std::make_shared<Camera>(m_width, m_height, glm::vec3(0.0f, 0.0f, 2.0f));
    InitializeImGui();

    return true;
}

void Window::SetupViewport() {
    // Crear framebuffer
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    // Crear textura para el color buffer
    glGenTextures(1, &m_viewportTexture);
    glBindTexture(GL_TEXTURE_2D, m_viewportTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_viewportSize.x, m_viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_viewportTexture, 0);

    // Crear renderbuffer para depth y stencil
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_viewportSize.x, m_viewportSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::ResizeViewport() {
    // Actualizar textura y renderbuffer con el nuevo tama�o
    glBindTexture(GL_TEXTURE_2D, m_viewportTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_viewportSize.x, m_viewportSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_viewportSize.x, m_viewportSize.y);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::BeginViewportRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::EndViewportRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::CleanupViewport() {
    if (m_framebuffer) {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = 0;
    }
    if (m_viewportTexture) {
        glDeleteTextures(1, &m_viewportTexture);
        m_viewportTexture = 0;
    }
    if (m_rbo) {
        glDeleteRenderbuffers(1, &m_rbo);
        m_rbo = 0;
    }
}

void Window::Run() {
    if (!m_window) {
        throw std::runtime_error("Window not initialized before calling Run()");
    }

    while (!glfwWindowShouldClose(m_window)) {
        Update();
    }
}

void Window::Update() {
    // Process input
    ProcessInput();

    // Clear screen
    glClearColor(0.21f, 0.35f, 0.42f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void Window::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    m_imguiInitialized = true;
}

void Window::ProcessInput() {
    if (!m_camera) return;

    m_camera->KeyboardInputs(m_window);

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        m_camera->MouseInputs(m_window);
    }

    m_camera->updateMatrix(45.0f, 0.01f, 100.0f);
}

void Window::RenderImGui() {
    if (!m_camera) return;

    ImGui::Begin("Metrics");

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / io.Framerate, io.Framerate);

    const auto& position = m_camera->Position;
    const auto& orientation = m_camera->Orientation;

    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    ImGui::Text("Camera Orientation: (%.2f, %.2f, %.2f)", orientation.x, orientation.y, orientation.z);

    ImGui::End();

    ImGui::Begin("Viewport");
    {
        // Mostrar la textura del viewport en ImGui
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (viewportSize.x != m_viewportSize.x || viewportSize.y != m_viewportSize.y) {
            m_viewportSize = { viewportSize.x, viewportSize.y };
            ResizeViewport();
        }

        // Usar la textura del framebuffer como imagen ImGui
        ImGui::Image(
            (ImTextureID)(intptr_t)GetViewportTexture(),
            viewportSize,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
    }
    ImGui::End();
}

void Window::Cleanup() {
    CleanupViewport();

    if (m_imguiInitialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        m_imguiInitialized = false;
    }

    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    
    glfwTerminate();
}

void Window::SetFramebufferSizeCallback(GLFWframebuffersizefun callback) {
    if (m_window) {
        glfwSetFramebufferSizeCallback(m_window, callback);
    }
}

void Window::SetKeyCallback(GLFWkeyfun callback) {
    if (m_window) {
        glfwSetKeyCallback(m_window, callback);
    }
}