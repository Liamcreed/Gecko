#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
namespace Gecko
{
    class Window
    {
    private:
        std::string m_Title = "Application";
        int m_Width, m_Height;
        GLFWwindow *m_GLFWWindow;

    private:
        void Clean();
    public:
        void Update();
          
        Window(std::string t, int w, int h);
        ~Window();
    public:
        bool Closed() { return glfwWindowShouldClose(m_GLFWWindow); }

        int GetWidth() { return m_Width; }
        int GetHeight() { return m_Height; }
        std::string &GetTitle() { return m_Title; }
        GLFWwindow *GetGLFWWindow() { return m_GLFWWindow; }

        void SetSize(glm::vec2 size)
        {
            m_Width = size.x;
            m_Height = size.y;
            glfwSetWindowSize(m_GLFWWindow, m_Width, m_Height);
        }
        void SetPosition(glm::vec2 pos)
        {
            glfwSetWindowPos(m_GLFWWindow, pos.x, pos.y);
        }
        void SetTitle(const std::string &title)
        {
            m_Title = title;
            glfwSetWindowTitle(m_GLFWWindow, title.c_str());
        }
        void Vsync(bool vsync)
        {
            if(vsync)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);
        }
        void Fullscreen(bool fullscreen)
        {
            if (fullscreen)
            {
                std::cout << BOLDYELLOW << "Fullscreen not available!" << RESET << std::endl;
            }
        }

        void CursorDisabled(bool disabled)
        {
            if (disabled)
                glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else
                glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    };

} // namespace Gecko