#include "gkpch.h"
#include "PerspectiveCameraController.h"
#include "Gecko/Core/Input.h"

namespace Gecko
{
    PerspectiveCameraController::PerspectiveCameraController(int w, int h)
    {
        m_PerspectiveCamera.SetProjection(w, h);
    }
    void PerspectiveCameraController::OnUpdate()
    {
        m_PerspectiveCamera.SetCameraView(m_PerspectiveCamera.GetPosition(), m_PerspectiveCamera.GetPosition() + m_PerspectiveCamera.GetFront(), m_PerspectiveCamera.GetUp());

        glm::vec3 front;
        front.x = cos(glm::radians(m_PerspectiveCamera.GetYaw())) * cos(glm::radians(m_PerspectiveCamera.GetPitch()));
        front.y = sin(glm::radians(m_PerspectiveCamera.GetPitch()));
        front.z = sin(glm::radians(m_PerspectiveCamera.GetYaw())) * cos(glm::radians(m_PerspectiveCamera.GetPitch()));
        m_PerspectiveCamera.GetFront() = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_PerspectiveCamera.GetRight() = glm::normalize(glm::cross(m_PerspectiveCamera.GetFront(), m_PerspectiveCamera.GetWorldUp())); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_PerspectiveCamera.GetUp() = glm::normalize(glm::cross(m_PerspectiveCamera.GetRight(), m_PerspectiveCamera.GetFront()));

        static float lastX = Application::Get()->GetWindow()->GetWidth() / 2.0f;
        static float lastY = Application::Get()->GetWindow()->GetHeight() / 2.0f;

        if (firstMouse)
        {
            lastX = Input::MousePos().x;
            lastY = Input::MousePos().y;
            firstMouse = false;
        }
        if (Input::MouseButtonPressed(GK_MOUSE_BUTTON_LEFT))
        {
            float xoffset = Input::MousePos().x - lastX;
            float yoffset = lastY - Input::MousePos().y; // reversed since y-coordinates go from bottom to top
            lastX = Input::MousePos().x;
            lastY = Input::MousePos().y;

            if (Input::KeyPressed(GK_KEY_LEFT_SHIFT))
            {
                xoffset *= 0.005f;
                yoffset *= 0.005f;
                m_PerspectiveCamera.GetPosition() -= m_PerspectiveCamera.GetUp() * yoffset;
                m_PerspectiveCamera.GetPosition() -= m_PerspectiveCamera.GetRight() * xoffset;
            }
            else if (Input::KeyPressed(GK_KEY_LEFT_CONTROL))
            {
                float pitch = m_PerspectiveCamera.GetPitch();
                float yaw = m_PerspectiveCamera.GetYaw();
                xoffset *= 0.1f;
                yoffset *= 0.1f;

                m_PerspectiveCamera.GetPitch() -= yoffset;
                m_PerspectiveCamera.GetYaw() -= xoffset;
            }
        }
        else
            firstMouse = true;

        m_PerspectiveCamera.GetPosition() += m_PerspectiveCamera.GetFront() * (float)Input::ScrollWheel().x * 0.1f;
    }
    void PerspectiveCameraController::OnResize(int w, int h)
    {
        m_PerspectiveCamera.SetProjection(w, h);
    }
    PerspectiveCameraController::~PerspectiveCameraController()
    {
    }
} // namespace Gecko