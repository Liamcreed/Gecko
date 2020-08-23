#include "gkpch.h"
#include "Shader.h"

namespace Gecko
{
    Shader::Shader(std::string const &vertexPath, std::string const &fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vertexFile;
        std::ifstream fragmentFile;

        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);
            std::stringstream vertexStream, fragmentStream;

            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        }
        catch (const std::exception &e)
        {
            GK_LOG(GK_ERROR) << "Shader file not succesfully read: " << vertexPath << " " << fragmentPath << e.what() << '\n';
        }
        const char *vertexCodeC = vertexCode.c_str();
        const char *fragmentCodeC = fragmentCode.c_str();

        //Compile and check for errors
        uint32_t vertex, fragment;
        int succes;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCodeC, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
        if (!succes)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            GK_LOG(BOLDRED "[VERTEX SHADER COMPILE ERROR]" RESET) << infoLog << std::endl;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCodeC, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
        if (!succes)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            GK_LOG(BOLDRED "[FRAGMENT SHADER COMPILE ERROR]" RESET) << infoLog << std::endl;
        }

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertex);
        glAttachShader(m_RendererID, fragment);
        glLinkProgram(m_RendererID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void Shader::Bind()
    {
        glUseProgram(m_RendererID);
    }
    void Shader::SetBool(const std::string &name, bool value)
    {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
    }
    void Shader::SetInt(const std::string &name, int value)
    {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void Shader::SetFloat(const std::string &name, float value)
    {
        glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
    }
    void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
} // namespace Gecko
