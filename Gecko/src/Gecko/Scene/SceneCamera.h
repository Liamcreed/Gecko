#pragma once
#include "Gecko/Renderer/Camera.h"

namespace Gecko
{
    class SceneCamera : public Camera
    {
    private:
        float m_PerspectiveNear = 0.1f, m_PerspectiveFar = 100.0f;
        float m_PerspectiveFOV = 45.0f;

        float m_AspectRatio = 0.0f;

    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetPerspective(float fov, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
        void SetPerspectiveFOV(float fov)
        {
            m_PerspectiveFOV = fov;
            RecalculateProjection();
        }

    private:
        void RecalculateProjection();
    };

} // namespace Gecko
