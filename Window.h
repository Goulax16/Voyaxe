#pragma once

#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include <thread>

#include "Node.h"

// Forward Decl
class Camera;

class Window {
public:
    using RenderingCallback = std::function<void()>;

    void AddRenderCallback(RenderingCallback callback);
    void RemoveAllRenderCallbacks();

    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    bool Initialize();
    void Run();
    void Update();

    GLFWwindow* GetNativeWindow() const { return m_window; }
    operator GLFWwindow* () const { return m_window; }

    std::shared_ptr<Camera> GetCamera() const { return m_camera; }

    Camera& GetGlobalCamera() { return *m_camera; }

    glm::ivec2 GetSize() const { return { m_width, m_height }; }
    float GetAspectRatio() const { return static_cast<float>(m_width) / m_height; }

    std::vector<std::weak_ptr<Node>> m_nodeList;

    bool vSync = true;
    void EnableVSync(bool state);

private:
    void InitializeImGui();
    void RenderImGui();
    void Cleanup();
    void ProcessInput(float dt);

    void RenderNodeTree(Node* node);

    GLFWwindow* m_window = nullptr;
    int m_width;
    int m_height;
    std::string m_title;

    void CaptureWindowToTexture();

    std::shared_ptr<Camera> m_camera;

    bool m_imguiInitialized = false;

    std::vector<RenderingCallback> m_renderCallbacks;

    unsigned int m_captureFramebuffer = 0;
    unsigned int m_captureTexture = 0;
};