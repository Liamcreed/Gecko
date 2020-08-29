#pragma once
#include <glad/glad.h>
#include <assert.h>
#include <glm/glm.hpp>

namespace Gecko
{
    class FrameBuffer
    {
    private:
        uint32_t m_Width, m_Height;
        uint32_t m_ColorAttachment;
        uint32_t m_RendererID;
        uint32_t m_RenderBuffer;

    public:
        FrameBuffer(uint32_t width, uint32_t height);
        glm::vec2 GetSize() { return glm::vec2(m_Width, m_Height); }
        void Invalidate();
        void Bind();
        void BindRenderBuffer() { glBindRenderbuffer(GL_RENDERBUFFER, m_ColorAttachment); }
        void UnBind();
        void Resize(uint32_t width, uint32_t height);
        uint32_t GetColorAttachmentRendererID()
        {
            glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
            return m_ColorAttachment;
        }
        ~FrameBuffer();
    };

} // namespace Gecko
