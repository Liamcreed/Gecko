#include "FrameBuffer.h"
#include "gkpch.h"
namespace Gecko
{
    FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height)
    {

        GLCall(glGenFramebuffers(1, &m_RendererID));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

        GLCall(glGenTextures(1, &m_ColorAttachment));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

        GLCall(glGenRenderbuffers(1, &m_RenderBuffer));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            GK_LOG(GK_ERROR) << "Framebuffer is not complete!" << std::endl;
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
    void FrameBuffer::Invalidate()
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_ColorAttachment));
        GLCall(glDeleteRenderbuffers(1, &m_RenderBuffer));

        GLCall(glGenFramebuffers(1, &m_RendererID));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

        GLCall(glGenTextures(1, &m_ColorAttachment));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

        GLCall(glGenRenderbuffers(1, &m_RenderBuffer));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            GK_LOG(GK_ERROR) << "Framebuffer incomplete!\n";
    }
    void FrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, m_Width, m_Height);
    }
    void FrameBuffer::UnBind()
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        GLCall(glDisable(GL_DEPTH_TEST));
    }
    void FrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RED, GL_FLOAT, NULL));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
    }
    FrameBuffer::~FrameBuffer()
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_ColorAttachment));
        GLCall(glDeleteRenderbuffers(1, &m_RenderBuffer));
    }
} // namespace Gecko