#include "gkpch.h"
#include "PerspectiveCamera.h"
#include "Gecko/Core/Application.h"
#include "Gecko/Core/Input.h"
namespace Gecko
{
    PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up, float yaw, float pitch)
        : m_LookAt(std::move(lookat))

    {
        this->m_Position = position;
        this->m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
        m_LookAt = lookat;
        m_Front =  glm::vec3(0.0f, 0.0f, -1.0f);
        
        //m_ViewMatrix = glm::lookAt(position, position + m_Front, up);
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), (float)Application::Get()->GetWindow()->GetWidth() / (float)Application::Get()->GetWindow()->GetHeight(), m_Near, m_Far);
    }
    PerspectiveCamera::~PerspectiveCamera()
    {
    }
    void PerspectiveCamera::SetProjection(float w, float h)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), w / h, m_Near, m_Far);
    }
    void PerspectiveCamera::SetCameraView(glm::vec3 position, glm::vec3 lookat, glm::vec3 up)
    {
        this->m_Position = position;
        this->m_LookAt = lookat;
        this->m_Up = up;
        m_ViewMatrix = glm::lookAt(position, m_LookAt, up);
    }

} // namespace Gecko