#pragma once
#include "Window.h"
#include "Application.h"
#include "KeyCodes.h"

#define MAX_KEYS 1024
#define MAX_BUTTONS 1024

namespace Gecko
{
    class Input
    {
    public:
        static bool Keys[MAX_KEYS];
        static bool MouseButtons[MAX_BUTTONS];

        static double MouseX, MouseY;
        static float ScrollX, ScrollY;
        static int Action;

        static bool KeyPressed(int key);
        static bool KeyReleased(int key);
        static bool KeyPress(int key);

        static int GetAxis(std::string axis);

        static bool MouseButtonPressed(int button);
        static bool MouseButtonReleased(int button);
        static bool MouseButtonPress(int button);

        static glm::vec2 MousePos() { return glm::vec2(MouseX, MouseY); }
        static glm::vec2 ScrollWheel();
        static glm::vec2 MouseDragOffset();

    public:
    };

} // namespace Gecko
