#include "gkpch.h"
#include "Input.h"
#include "KeyCodes.h"

namespace Gecko
{
    bool Input::Keys[MAX_KEYS];
    bool Input::MouseButtons[MAX_BUTTONS];

    double Input::MouseX, Input::MouseY;
    float Input::ScrollX, Input::ScrollY;
    int Input::Action;

    bool Input::KeyPressed(int key)
    {
        if (Keys[key] == true)
            return true;
        return false;
    }
    /* Only returns true when key is pressed once!*/
    bool Input::KeyPress(int key)
    {
        static bool pressed = false;

        if (Keys[key] == true && pressed == false)
        {
            pressed = true;
            return true;
        }
        else if (Keys[key] == false)
        {
            pressed = false;
        }
        return false;
    }
    bool Input::KeyReleased(int key)
    {
        if (Keys[key] == false)
            return true;
        return false;
    }
    bool Input::MouseButtonPressed(int button)
    {
        if (MouseButtons[button] == true)
            return true;
        return false;
    }
    bool Input::MouseButtonReleased(int button)
    {
        if (MouseButtons[button] == false)
            return true;
        return false;
    }
    /* Only returns true when mouse is pressed once!*/
    bool Input::MouseButtonPress(int button)
    {
        static bool pressed = false;

        if (Input::MouseButtons[button] == true && pressed == false)
        {
            pressed = true;
            return true;
        }
        else if (MouseButtons[button] == false)
        {
            pressed = false;
        }
        return false;
    }
    int Input::GetAxis(std::string axis)
    {
        if (axis == "Horizontal")
        {
            if (Keys[GK_KEY_A] | Keys[GK_KEY_LEFT])
                return -1;
            if (Keys[GK_KEY_D] | Keys[GK_KEY_RIGHT])
                return 1;
        }
        else if (axis == "Vertical")
        {
            if (Keys[GK_KEY_W] | Keys[GK_KEY_UP])
                return 1;
            if (Keys[GK_KEY_S] | Keys[GK_KEY_DOWN])
                return -1;
        }
        else
        {
            GK_LOG(GK_ERROR) << axis << " is not an availible axis!\n";
        }

        return 0;
    }
    glm::vec2 Input::ScrollWheel()
    {
        int x, y;
        float threshold = 0.1;

        if (ScrollX > threshold)
            x = 1;
        else if (ScrollX < -threshold)
            x = -1;
        else
            x = 0;

        if (ScrollY > threshold)
            y = 1;
        else if (ScrollY < -threshold)
            y = -1;
        else
            y = 0;

        return glm::vec2(x, y);
    }
    glm::vec2 Input::MouseDragOffset()
    {
        static glm::vec2 lastPos = Input::MousePos();
        glm::vec2 result = Input::MousePos() - lastPos;
        lastPos = Input::MousePos();
        return result;
    }
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        Input::ScrollX = xoffset;
        Input::ScrollY = yoffset;
    }
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Input::Action = action;
        Input::Keys[key] = Input::Action != GLFW_RELEASE;
    }
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        Input::MouseX = xpos;
        Input::MouseY = ypos;
    }
    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        Input::MouseButtons[button] = action != GLFW_RELEASE;
    }
} // namespace Gecko