#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <raymath.h>
#include <cassert>
#include <iostream>
#include "Shader.h"

void APIENTRY DebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

struct App
{
    int object_index;
};

int main(void)
{
    App app{};

    assert(glfwInit() == GLFW_TRUE);

    // Request certain OpenGL features
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef NDEBUG
    // Don't make a debug context if in release mode
#else
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Graphics 1", nullptr, nullptr);
    assert(window != nullptr);

    glfwSetWindowUserPointer(window, &app);
    glfwSetKeyCallback(window, KeyCallback);

    glfwMakeContextCurrent(window);
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == true);

#ifdef NDEBUG
#else
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallback, nullptr);
#endif

    // Face-culling is disabled by default, but we should remember that OpenGL defines front-faces as CCW winding-order
    Vector3 triangle_positions[] =
    {
        {  0.0f,  0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f }
    };

    Vector3 triangle_colors[] =
    {
        {  1.0f, 0.0f, 0.0 },
        {  0.0f, 1.0f, 0.0 },
        {  0.0f, 0.0f, 1.0 }
    };

    GLuint triangle_vao = GL_NONE;
    GLuint triangle_positions_vbo = GL_NONE;
    GLuint triangle_colors_vbo = GL_NONE;

    glGenVertexArrays(1, &triangle_vao);
    glGenBuffers(1, &triangle_positions_vbo);
    glGenBuffers(1, &triangle_colors_vbo);

    glBindVertexArray(triangle_vao);                            // "Record the following state of vertex buffers within this vertex array"

    glBindBuffer(GL_ARRAY_BUFFER, triangle_positions_vbo);      // The following array (vertex) buffer will be described (triangle_positions_vbo):
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);// Index 0, 3 components, float data, no implicit normalization, no stride, no offset
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vector3), triangle_positions, GL_STATIC_DRAW); // Transfer memory of triangle_positions into currently bound vbo
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, triangle_colors_vbo);         // The following array (vertex) buffer will be described (triangle_colors_vbo):
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);// Index 1, 3 components, float data, no implicit normalization, no stride, no offset
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vector3), triangle_colors, GL_STATIC_DRAW); // Transfer memory of triangle_colors into currently bound vbo
    glEnableVertexAttribArray(1);

    glBindVertexArray(GL_NONE);                                 // "Stop recording vertex buffer state"

    GLuint vs_shader = LoadShader(GL_VERTEX_SHADER, "./assets/shaders/a2_lines.vert");
    GLuint fs_shader = LoadShader(GL_FRAGMENT_SHADER, "./assets/shaders/a2_lines.frag");
    GLuint shader_program = LoadProgram(vs_shader, fs_shader);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(triangle_vao);
        glDrawArrays(GL_LINE_LOOP, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    App* app = (App*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        // Bound our array to be [0..4]
        ++app->object_index %= 5;
    }
}

void APIENTRY DebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
