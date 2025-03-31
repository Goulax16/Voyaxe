#include "Shader.h"
#include "EBO.h"
#include "VAO.h"
#include "Texture.h"
#include "Math.h"
#include "Camera.h"
#include "Node3D.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

int main() {
	//Window* window = new Window(800, 600, "Hello");
	//
	//window->Run();

    auto* node = new Node3D();

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    glViewport(0, 0, 800, 600);

    Shader shader = Shader("vert.glsl", "frag.glsl");

    VAO vao;
    vao.Bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    Texture brickTex("test.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.texUnit(shader, "tex0", 0);

    GLuint quatLoc = glGetUniformLocation(shader.ID, "rotationQuat");

    float angle = 0.0f;

    glEnable(GL_DEPTH_TEST);

    Camera camera(800, 600, glm::vec3(0.0f, 0.0f, 2.0f));

    node->Init();

    while (!glfwWindowShouldClose(window)) {
        angle += 0.01f;

        glClearColor(0.0f, 0.24f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Activate();

        Quaternion rotQuat = eulerToQuaternion(angle, 0.0f, 0.0f);  
        glUniform4f(quatLoc, rotQuat.x, rotQuat.y, rotQuat.z, rotQuat.w);

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");

        brickTex.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.Delete();
    vbo.Delete();
    ebo.Delete();
    brickTex.Delete();
    shader.Delete();

    delete node;

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}