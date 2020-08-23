#include "gkpch.h"
#include "Input.h"
#include "Window.h"

namespace Gecko
{
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    Window::Window(const std::string t, int w, int h)
        : m_Title(t), m_Width(w), m_Height(h)
    {
        if (!glfwInit())
        {
            glfwTerminate();
            GK_LOG(GK_ERROR) << "Failed to initialize GLFW!\n";
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        m_GLFWWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

        if (m_GLFWWindow == nullptr)
        {
            glfwTerminate();
            GK_LOG(GK_ERROR) << "Failed to create GLFWWindow!\n";
        }
        glfwMakeContextCurrent(m_GLFWWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            GK_LOG(GK_ERROR) << "Failed to initialize GLAD!\n";
        }
        glfwSwapInterval(1);

        glfwSetKeyCallback(m_GLFWWindow, key_callback);
        glfwSetCursorPosCallback(m_GLFWWindow, cursor_position_callback);
        glfwSetMouseButtonCallback(m_GLFWWindow, mouse_button_callback);
        glfwSetScrollCallback(m_GLFWWindow, scroll_callback);
    }
    void Window::Update()
    {
        glfwPollEvents();
        glfwGetWindowSize(m_GLFWWindow, &m_Width, &m_Height);
        glfwSwapBuffers(m_GLFWWindow);
    }
    void Window::Clean()
    {
        glfwDestroyWindow(m_GLFWWindow);
        glfwTerminate();
    }
    Window::~Window()
    {
        Clean();
    }
} // namespace Gecko