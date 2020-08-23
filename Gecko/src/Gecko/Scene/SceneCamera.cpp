#include "gkpch.h"
#include "SceneCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Gecko
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();
    }
    void SceneCamera::SetPerspective(float  fov, float nearClip, float farClip)
    {
        m_PerspectiveNear = nearClip;  m_PerspectiveFar = farClip;
        m_PerspectiveFOV = fov;

        RecalculateProjection();
    }
    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width/(float)height;
        RecalculateProjection();
    }
    void SceneCamera::RecalculateProjection()
    {
        m_Projection =  glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
    }
} // namespace Gecko
