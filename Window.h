#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera; // Forward declaration

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Delete copy operations
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Allow move operations
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    bool Initialize();
    void Run();
    void Update();  // Maintained from original

    // Accessors
    GLFWwindow* GetNativeWindow() const { return m_window; }
    operator GLFWwindow* () const { return m_window; }

    std::shared_ptr<Camera> GetCamera() const { return m_camera; }
    Camera& GetGlobalCamera() { return *m_camera; }  // Maintained from original
    glm::ivec2 GetSize() const { return { m_width, m_height }; }
    float GetAspectRatio() const { return static_cast<float>(m_width) / m_height; }

    // Callbacks
    void SetFramebufferSizeCallback(GLFWframebuffersizefun callback);
    void SetKeyCallback(GLFWkeyfun callback);

    void BeginViewportRender();
    void EndViewportRender();
    unsigned int GetViewportTexture() const { return m_viewportTexture; }
    glm::vec2 GetViewportSize() const { return m_viewportSize; }
    void SetViewportSize(const glm::vec2& size) { m_viewportSize = size; ResizeViewport(); }

private:
    void InitializeImGui();
    void RenderImGui();
    void Cleanup();
    void ProcessInput();

    GLFWwindow* m_window = nullptr;
    int m_width;
    int m_height;
    std::string m_title;

    std::shared_ptr<Camera> m_camera;
    bool m_imguiInitialized = false;

    void SetupViewport();
    void ResizeViewport();
    void CleanupViewport();

    unsigned int m_framebuffer = 0;
    unsigned int m_viewportTexture = 0;
    unsigned int m_rbo = 0;
    glm::vec2 m_viewportSize = { 0.0f, 0.0f };
};

#endif // WINDOW_H