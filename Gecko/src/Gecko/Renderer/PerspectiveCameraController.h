#pragma once
#include "PerspectiveCamera.h"

namespace Gecko
{
    class PerspectiveCameraController
    {
    private:
        PerspectiveCamera m_PerspectiveCamera{glm::vec3(1.0f, 1.0f, 3.0f), glm::vec3(0.0f, 0.0f, 3.0f)};
    public:
        bool firstMouse;
        PerspectiveCameraController(int w,  int h);
        ~PerspectiveCameraController();
        void OnUpdate();
        void OnResize(int w,  int h);
        PerspectiveCamera& GetCamera(){return m_PerspectiveCamera;}
    };
    
    
}