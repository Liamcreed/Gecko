#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Gecko
{
    class Shader
    {
    private:
        uint32_t m_RendererID;

    public:
        Shader(std::string const &vertexPath, std::string const &fragmentPath);
        void Bind();
        void Unbind() { glUseProgram(0); }
        void SetBool(const std::string &name, bool value);
        void SetInt(const std::string &name, int value);
        void SetFloat(const std::string &name, float value);
        void SetVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void SetVec3(const std::string &name, const glm::vec3 &value) const;
        void SetVec3(const std::string &name, float x, float y, float z) const;
        void SetVec4(const std::string &name, const glm::vec4 &value) const;
        void SetVec4(const std::string &name, float x, float y, float z, float w) const;
        void SetMat2(const std::string &name, const glm::mat2 &mat) const;
        void SetMat3(const std::string &name, const glm::mat3 &mat) const;
        void SetMat4(const std::string &name, const glm::mat4 &mat) const;

        uint32_t GetID() { return m_RendererID; }
    };
} // namespace Gecko
