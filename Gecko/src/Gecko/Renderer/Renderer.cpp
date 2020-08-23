#include "gkpch.h"
#include "Renderer.h"

namespace Gecko
{
    struct RendererData
    {
        glm::mat4 ProjectionMatrix;
        glm::mat4 TransformMatrix;
        glm::mat4 ViewMatrix;
        Ref<Shader> shader;
    };
    static RendererData s_Data;

    Renderer::Renderer()
    {
    }
    void Renderer::Init()
    {
        glfwSwapInterval(1);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    void Renderer::Clear(glm::vec4 color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void Renderer::BeginScene(Camera &camera, const glm::mat4 &transform)
    {
        glm::mat4 view = glm::inverse(transform);
        glm::mat4 proj = camera.GetProjection();

        s_Data.ProjectionMatrix = proj;
        s_Data.ViewMatrix = view;

        s_Data.shader->Bind();
        s_Data.shader->SetMat4("projection", proj);
        s_Data.shader->SetMat4("view", view);
    }

    void Renderer::BeginScene(PerspectiveCamera &camera)
    {
        s_Data.ProjectionMatrix = camera.GetProjectionMatrix();
        s_Data.ViewMatrix = camera.GetViewMatrix();

        s_Data.shader->Bind();
        s_Data.shader->SetMat4("projection", camera.GetProjectionMatrix());
        s_Data.shader->SetMat4("view", camera.GetViewMatrix());
    }
    void Renderer::EndScene()
    {
    }
    void Renderer::Submit(const Ref<Shader> &shader, const glm::mat4 transform)
    {
        s_Data.shader = shader;
        s_Data.TransformMatrix = transform;
    }
    void Renderer::DrawModel(Ref<Model> &model, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation)
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        s_Data.shader->SetMat4("model", transform);

        model->Draw(s_Data.shader);
    }
    void DrawMesh(Ref<Mesh>& mesh,const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation )
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        s_Data.shader->SetMat4("model", transform);
        mesh->Draw(s_Data.shader);
    }
    void Renderer::DrawCube(Ref<Texture> &texture, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation)
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::scale(transform, scale);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        texture->Bind();
        s_Data.shader->SetMat4("model", transform);
        //FIXME:
        s_Data.shader->SetFloat("material.texture_diffuse", 0);

        static uint32_t vao;
        static uint32_t vbo;

        if (vao == 0)
        {
            float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
                1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
                1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
                -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
                // front face
                -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
                1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
                1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
                -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
                -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
                -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
                -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                                    // right face
                1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
                1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
                1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
                1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
                1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
                1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
                // bottom face
                -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
                1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
                1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
                -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
                -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
                1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
                1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
                1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
                -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
                -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
            };
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // link vertex attributes
            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        // render Cube
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    Renderer::~Renderer()
    {
    }
} // namespace Gecko
