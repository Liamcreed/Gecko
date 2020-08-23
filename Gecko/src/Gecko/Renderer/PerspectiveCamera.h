#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Gecko/Core/Application.h"

namespace Gecko
{
    class PerspectiveCamera
    {
    private:
        float m_FOV = 45.0f;

    private:
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::vec3 m_LookAt;

        glm::vec3 m_Position;
        glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;

    private:
        float m_Yaw;
        float m_Pitch;

    private:
        //Euler angles
        float m_Near = 0.1f, m_Far = 100.0f;

    public:
        PerspectiveCamera(glm::vec3 position, glm::vec3 lookat, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
        ~PerspectiveCamera();
        void SetProjection(float w, float h);
        void SetCameraView(glm::vec3 position, glm::vec3 lookat, glm::vec3 up);

    public:
        float &GetYaw() { return m_Yaw; }
        float &GetPitch() { return m_Pitch; }
        float &GetFOV() { return m_FOV; }

        glm::vec3 &GetPosition() { return m_Position; }
        glm::vec3 &GetRight() { return m_Right; }
        glm::vec3 &GetFront() { return m_Front; }
        glm::vec3 &GetWorldUp() { return m_WorldUp; }
        glm::vec3 &GetUp() { return m_Up; }
        glm::vec3 &GetLookAt() { return m_LookAt; }
        glm::mat4 &GetProjectionMatrix() { return m_ProjectionMatrix; }
        glm::mat4 &GetViewMatrix() { return m_ViewMatrix; }
    };
} // namespace Gecko
